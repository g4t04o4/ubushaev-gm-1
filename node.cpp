#include "node.h"
#include "edge.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graphWidget, unsigned int ID)
    : nodeID(ID), graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::getEdges() const
{
    return edgeList;
}

unsigned int Node::getID() const
{
    return nodeID;
}

void Node::calculeteForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        newPos = pos();
        return;
    }

    /*
     * Рассчёт сил, действующих на узел.
     */
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items())
    {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node) continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    /*
     * Объединение всех "сил" в одно тянущее усилие.
     */
    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList)
    {
        QPointF vec;
        if (edge->getSourceNode() == this)
            vec = mapToItem(edge->getDestNode(), 0, 0);
        else
            vec = mapToItem(edge->getSourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10),
                     sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10),
                     sceneRect.bottom() - 10));
}

bool Node::advancePosition()
{
    if (newPos == pos()) return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust,
                   23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter * painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::red).light(120));
        gradient.setColorAt(0, QColor(Qt::darkRed).light(120));
    }
    else
    {
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(1, Qt::darkRed);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    switch (change)
    {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
