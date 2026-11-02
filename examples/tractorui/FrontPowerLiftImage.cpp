/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "FrontPowerLiftImage.h"
#include "PowerLiftArc.h"

#include <QskGraphicLabel.h>

#include <QQuickItem>
#include <QMatrix4x4>
#include <QTransform>
#include <qmath.h>

namespace
{
    // the tractor.svg viewBox, so the articulation points below can be given in
    // the drawing's own units and turned into fractions of the item
    constexpr qreal SvgW = 228.5387;
    constexpr qreal SvgH = 137.39494;

    // the loader boom hinge pin, as a fraction of the viewBox — the joint at
    // ~( 135, 50 ) in SVG units
    constexpr qreal PivotFx = 135.0 / SvgW;
    constexpr qreal PivotFy = 50.0 / SvgH;

    // the upper arm endpoints: A = anchor on the static 3-bolt plate,
    // B = far end on the bell-crank ( moves with the boom ), viewBox fractions
    constexpr qreal ArmAx = 140.0 / SvgW, ArmAy = 23.0 / SvgH;
    constexpr qreal ArmBx = 177.0 / SvgW, ArmBy = 29.0 / SvgH;

    // the bucket, as a fraction of the viewBox — the point kept level with the
    // arc handle. An effective point: the scoop is a curved shape, these were
    // tuned so the rendered scoop sits on the handle across its travel.
    constexpr qreal BucketFx = 200.0 / SvgW;
    constexpr qreal BucketFy = 46.0 / SvgH;

    // The boom is rotated about the pivot to raise / lower the bucket. The three
    // layers fill the item with fillMode = Stretch, so a point at viewBox
    // fraction ( fx, fy ) is drawn at ( fx·w, fy·h ); rotating by θ about the
    // pivot puts the bucket's vertical position ( as a fraction of the item
    // height ) at
    //     PivotFy + ( BucketFx - PivotFx )·( w / h )·sinθ + ( BucketFy - PivotFy )·cosθ
    // We do not hard-code the angle: given where the handle is ( targetFraction,
    // a fraction of the item height ) we solve for the boom angle that lands the
    // bucket there, from the *live* w and h. That keeps the bucket level with
    // the handle at any window size, which a fixed angle cannot. Inverting
    // a·sinθ + b·cosθ = c :
    qreal boomAngleForHandle( qreal targetFraction, qreal w, qreal h )
    {
        if ( h <= 0.0 )
            return 0.0;

        const qreal a = ( BucketFx - PivotFx ) * ( w / h );
        const qreal b = ( BucketFy - PivotFy );
        const qreal c = targetFraction - PivotFy;

        const qreal r = std::hypot( a, b );
        if ( r < 1e-9 )
            return 0.0;

        const qreal theta = std::asin( qBound( -1.0, c / r, 1.0 ) ) - std::atan2( b, a );
        return qBound( -15.0, qRadiansToDegrees( theta ), 95.0 );
    }

    // rotates its item around an arbitrary pivot, expressed as a fraction of
    // the item size so it survives resizing
    class LoaderRotation : public QQuickTransform
    {
      public:
        LoaderRotation( qreal fx, qreal fy, QQuickItem* item )
            : QQuickTransform( item )
            , m_fx( fx )
            , m_fy( fy )
        {
            prependToItem( item );
        }

        void setAngle( qreal degrees )
        {
            if ( !qFuzzyCompare( m_degrees, degrees ) )
            {
                m_degrees = degrees;
                update();
            }
        }

      protected:
        void applyTo( QMatrix4x4* matrix ) const override
        {
            if ( const auto item = qobject_cast< QQuickItem* >( parent() ) )
            {
                const qreal px = m_fx * item->width();
                const qreal py = m_fy * item->height();

                QTransform t;
                t.translate( px, py );
                t.rotate( m_degrees );
                t.translate( -px, -py );

                *matrix *= t;
            }
        }

      private:
        const qreal m_fx, m_fy;
        qreal m_degrees = 0.0;
    };

    // keeps the upper arm pinned at A ( on the static plate ) while its far end
    // follows B as the boom swings about the pivot P — the arm rotates about A
    // and stretches along its axis, like an extending hydraulic ram.
    class ArmStretch : public QQuickTransform
    {
      public:
        ArmStretch( QQuickItem* item )
            : QQuickTransform( item )
        {
            prependToItem( item );
        }

        void setAngle( qreal degrees )
        {
            if ( !qFuzzyCompare( m_degrees, degrees ) )
            {
                m_degrees = degrees;
                update();
            }
        }

      protected:
        void applyTo( QMatrix4x4* matrix ) const override
        {
            const auto item = qobject_cast< QQuickItem* >( parent() );
            if ( item == nullptr )
                return;

            const qreal w = item->width(), h = item->height();
            const QPointF A( ArmAx * w, ArmAy * h );
            const QPointF B( ArmBx * w, ArmBy * h );
            const QPointF P( PivotFx * w, PivotFy * h );

            // B' = B rotated around the boom pivot by the boom angle
            const qreal rad = qDegreesToRadians( m_degrees );
            const qreal cs = std::cos( rad ), sn = std::sin( rad );
            const QPointF d = B - P;
            const QPointF Bp( P.x() + d.x() * cs - d.y() * sn,
                P.y() + d.x() * sn + d.y() * cs );

            // similarity mapping A->A and B->B' ( rotate about A + axial stretch )
            const QPointF ab = B - A, abp = Bp - A;
            const qreal len0 = std::hypot( ab.x(), ab.y() );
            const qreal lenp = std::hypot( abp.x(), abp.y() );
            const qreal s = ( len0 > 1e-6 ) ? lenp / len0 : 1.0;

            QTransform t;
            t.translate( A.x(), A.y() );
            t.rotateRadians( std::atan2( abp.y(), abp.x() ) );
            t.scale( s, 1.0 );
            t.rotateRadians( -std::atan2( ab.y(), ab.x() ) );
            t.translate( -A.x(), -A.y() );

            *matrix *= t;
        }

      private:
        qreal m_degrees = 0.0;
    };
}

class FrontPowerLiftImage::PrivateData
{
  public:
    TractorLabel* body = nullptr;
    TractorLabel* loader = nullptr;
    TractorLabel* upper = nullptr;

    LoaderRotation* rotation = nullptr;
    ArmStretch* armStretch = nullptr;

    const PowerLiftArc* arc = nullptr;
};

FrontPowerLiftImage::FrontPowerLiftImage( QQuickItem* parent )
    : QskControl( parent )
    , m_data( new PrivateData )
{
    setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Preferred );
    setPolishOnResize( true );

    m_data->body = new TractorLabel( "tractor-body", this );
    m_data->body->setFillMode( QskGraphicLabel::Stretch );

    m_data->loader = new TractorLabel( "tractor-loader", this );
    m_data->loader->setFillMode( QskGraphicLabel::Stretch );
    m_data->loader->setPanel( false ); // transparent overlay

    m_data->upper = new TractorLabel( "tractor-upper", this );
    m_data->upper->setFillMode( QskGraphicLabel::Stretch );
    m_data->upper->setPanel( false );

    m_data->rotation = new LoaderRotation( PivotFx, PivotFy, m_data->loader );
    m_data->armStretch = new ArmStretch( m_data->upper );
}

FrontPowerLiftImage::~FrontPowerLiftImage() = default;

void FrontPowerLiftImage::setLoaderAngle( qreal degrees )
{
    m_data->rotation->setAngle( degrees );
    m_data->armStretch->setAngle( degrees );
}

void FrontPowerLiftImage::setArc( const PowerLiftArc* arc )
{
    m_data->arc = arc;
    updateFromArc();
}

void FrontPowerLiftImage::updateFromArc()
{
    if ( m_data->arc == nullptr )
        return;

    const auto r = contentsRect();
    if ( r.height() <= 0.0 )
        return; // not laid out yet - updateLayout() will call us again

    // the handle height, expressed as a fraction of the drawing's frame
    const auto frameTop = mapToScene( r.topLeft() ).y();
    const auto fraction = ( m_data->arc->handleCenterSceneY() - frameTop ) / r.height();

    setLoaderAngle( boomAngleForHandle( fraction, r.width(), r.height() ) );
}

void FrontPowerLiftImage::updateLayout()
{
    const auto r = contentsRect();
    m_data->body->setGeometry( r );
    m_data->loader->setGeometry( r );
    m_data->upper->setGeometry( r );

    updateFromArc();
}
