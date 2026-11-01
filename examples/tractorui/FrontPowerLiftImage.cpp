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
    // the loader boom hinge pin, as a fraction of the tractor.svg viewBox
    // ( 228.5387 x 137.39494 ) — the joint at ~(135,50) in SVG units
    constexpr qreal PivotFx = 135.0 / 228.5387;
    constexpr qreal PivotFy = 50.0 / 137.39494;

    // the upper arm endpoints: A = anchor on the static 3-bolt plate,
    // B = far end on the bell-crank ( moves with the boom ), viewBox fractions
    constexpr qreal ArmAx = 140.0 / 228.5387, ArmAy = 23.0 / 137.39494;
    constexpr qreal ArmBx = 177.0 / 228.5387, ArmBy = 29.0 / 137.39494;

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

void FrontPowerLiftImage::updateLayout()
{
    const auto r = contentsRect();
    m_data->body->setGeometry( r );
    m_data->loader->setGeometry( r );
    m_data->upper->setGeometry( r );
}
