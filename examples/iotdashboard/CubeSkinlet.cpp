/******************************************************************************
 * Copyright (C) 2022 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "CubeSkinlet.h"
#include "Cube.h"

#include <QskGraphic.h>
#include <QskGraphicNode.h>
#include <QskSGNode.h>

#include <QSGNode>
#include <QSGTransformNode>
#include <QTransform>

CubeSkinlet::CubeSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles( {
        FrontRole,
        LeftRole,
        RightRole,
    } );
}

CubeSkinlet::~CubeSkinlet()
{
}

QRectF CubeSkinlet::subControlRect( const QskSkinnable* skinnable,
    const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
//    auto* display = static_cast< const Cube* >( skinnable );
//    QRectF rect = contentsRect;

//    if( subControl == Cube::Groove || subControl == Cube::Panel )
//    {
//        return rect;
//    }
//    else if( subControl == Cube::ColdAndWarmArc )
//    {
//        return rect;
//    }

    return contentsRect;
}

QSGNode* CubeSkinlet::updateSubNode(
    const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto* cube = static_cast< const Cube* >( skinnable );

    switch( nodeRole )
    {
        case FrontRole:
        {
            return updateFrontNode( cube, node );
        }
        default:
        return nullptr;
    }

    return Inherited::updateSubNode( skinnable, nodeRole, node );
}

QSGNode* CubeSkinlet::updateFrontNode( const Cube* cube, QSGNode* node ) const
{
    node = QskSGNode::ensureNode< QSGNode >( node );

    QSGTransformNode* transformNodeFront, *transformNodeOther;

    QSGNode* graphicNodeFront, *graphicNodeOther;

    if( node->childCount() == 0 )
    {
        transformNodeFront = new QSGTransformNode;
        node->appendChildNode( transformNodeFront );

        graphicNodeFront = new QskGraphicNode;
        transformNodeFront->appendChildNode( graphicNodeFront );

        transformNodeOther = new QSGTransformNode;
        node->appendChildNode( transformNodeOther );

        graphicNodeOther = new QskGraphicNode;
        transformNodeOther->appendChildNode( graphicNodeOther );
    }
    else
    {
        transformNodeFront = QskSGNode::ensureNode< QSGTransformNode >( node->childAtIndex( 0 ) );
        graphicNodeFront = static_cast< QskGraphicNode* >( transformNodeFront->childAtIndex( 0 ) );

        transformNodeOther = QskSGNode::ensureNode< QSGTransformNode >( node->childAtIndex( 1 ) );
        graphicNodeOther = static_cast< QskGraphicNode* >( transformNodeOther->childAtIndex( 0 ) );
    }

    QTransform transformFront, transformOther;
    const auto cr = cube->contentsRect();
    const qreal ratio = cube->metric( Cube::FrontControl | QskAspect::Position );

    const bool scalingInitially = ( ratio < 1 - m_scale );
    const bool scalingAtEnd = ( ratio > m_scale );
    const bool rotating = !scalingInitially && !scalingAtEnd;

    const qreal scale = scalingInitially ? qMax( 1 - ratio, m_scale ) : qMax( ratio, m_scale );

    const QSizeF t( ( 1 - scale ) * cr.width() / 2, ( 1 - scale ) * cr.height() / 2 );

    transformFront.translate( t.width(), t.height() ); // center rect
    transformFront.scale( scale, scale );

    transformOther.translate( t.width(), t.height() ); // center rect
    transformOther.scale( scale, scale );

    const auto rotateRatio = ( ratio - ( 1 - m_scale ) ) / ( 1 - ( 1 - m_scale ) * 2 );

    if( rotating )
    {
        const qreal frontAngle = rotateRatio * 90;

        transformFront.translate( -cr.width() * rotateRatio, 0 );
        transformFront.translate( cr.width(), cr.height() / 2 );
        transformFront.rotate( frontAngle, Qt::YAxis );
        transformFront.translate( -cr.width(), -cr.height() / 2 );

        const qreal otherAngle = ( 1 - rotateRatio ) * -90;

        transformOther.translate( cr.width(), cr.height() / 2 );
        transformOther.translate( -cr.width() * rotateRatio, 0 );
        transformOther.rotate( otherAngle, Qt::YAxis );
        transformOther.translate( 0, -cr.height() / 2 );
    }

    transformNodeFront->setMatrix( transformFront );
    transformNodeOther->setMatrix( transformOther );

    if( scalingInitially )
    {
        const auto imageFront = cube->image( Cube::Front );
        const auto graphicFront = QskGraphic::fromImage( imageFront );
        updateGraphicNode( cube, graphicNodeFront, graphicFront, Cube::FrontControl );
    }
    else
    {
        const auto imageOther = cube->image( Cube::Right );
        const auto graphicOther = QskGraphic::fromImage( imageOther );
        updateGraphicNode( cube, graphicNodeOther, graphicOther, Cube::FrontControl );
    }

    return node;
}

#include "moc_CubeSkinlet.cpp"
