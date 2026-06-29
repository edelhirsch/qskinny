/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "FrontPowerLiftTile.h"

#include "PowerLiftArc.h"
#include "QuickAccessButton.h"

#include <QskFontRole.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>

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

    // the tractor body with the movable front loader overlaid on top
    class TractorView : public QskControl
    {
      public:
        TractorView( QQuickItem* parent = nullptr )
            : QskControl( parent )
        {
            setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Preferred );
            setPolishOnResize( true );

            m_body = new TractorLabel( "tractor-body", this );
            m_body->setFillMode( QskGraphicLabel::Stretch );

            m_loader = new TractorLabel( "tractor-loader", this );
            m_loader->setFillMode( QskGraphicLabel::Stretch );
            m_loader->setPanel( false ); // transparent overlay

            m_upper = new TractorLabel( "tractor-upper", this );
            m_upper->setFillMode( QskGraphicLabel::Stretch );
            m_upper->setPanel( false );

            m_rotation = new LoaderRotation( PivotFx, PivotFy, m_loader );
            m_armStretch = new ArmStretch( m_upper );
        }

        void setLoaderAngle( qreal degrees )
        {
            m_rotation->setAngle( degrees );
            m_armStretch->setAngle( degrees );
        }

      protected:
        void updateLayout() override
        {
            const auto r = contentsRect();
            m_body->setGeometry( r );
            m_loader->setGeometry( r );
            m_upper->setGeometry( r );
        }

      private:
        TractorLabel* m_body;
        TractorLabel* m_loader;
        TractorLabel* m_upper;
        LoaderRotation* m_rotation;
        ArmStretch* m_armStretch;
    };
}

FrontPowerLiftTile::FrontPowerLiftTile( QQuickItem* parent )
    : Tile( "front power lift", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );
    outerBox->setMargins( { 20, 20, 20, 20 } );
    outerBox->setSpacing( 35 );

    auto* tractor = new TractorView( outerBox );

    auto* frontArc = new PowerLiftArc( PowerLiftArc::Type::Front, outerBox );
    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );

    // The loader is articulated and driven through TractorView::setLoaderAngle(),
    // 0 = raised ( the drawing's home ), larger = lowered. Once the arc has a drag
    // interaction, hook it up, e.g.:
    //   connect( frontArc, &PowerLiftArc::valueChanged, tractor,
    //       [ tractor ]( qreal v ) { tractor->setLoaderAngle( ( 1.0 - v ) * 26.0 ); } );
    Q_UNUSED( tractor )

    setContent( outerBox );
}
