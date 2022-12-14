/*
 *  SPDX-FileCopyrightText: 2020 Carson Black <uhhadd@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "pagerouter.h"
#include "loggingcategory.h"
#include <QJSEngine>
#include <QJSValue>
#include <QJsonObject>
#include <QJsonValue>
#include <QQmlProperty>
#include <QQuickWindow>
#include <QTimer>
#include <qqmlpropertymap.h>

ParsedRoute *parseRoute(QJSValue value)
{
    if (value.isUndefined()) {
        return new ParsedRoute{};
    } else if (value.isString()) {
        return new ParsedRoute{value.toString(), QVariant()};
    } else {
        auto map = value.toVariant().value<QVariantMap>();
        map.remove(QStringLiteral("route"));
        map.remove(QStringLiteral("data"));
        return new ParsedRoute{value.property(QStringLiteral("route")).toString(), //
                               value.property(QStringLiteral("data")).toVariant(),
                               map,
                               false};
    }
}

QList<ParsedRoute *> parseRoutes(QJSValue values)
{
    QList<ParsedRoute *> ret;
    if (values.isArray()) {
        const auto valuesList = values.toVariant().toList();
        for (const auto &route : valuesList) {
            if (route.toString() != QString()) {
                ret << new ParsedRoute{route.toString(), QVariant(), QVariantMap(), false, nullptr};
            } else if (route.canConvert<QVariantMap>()) {
                auto map = route.value<QVariantMap>();
                auto copy = map;
                copy.remove(QStringLiteral("route"));
                copy.remove(QStringLiteral("data"));

                ret << new ParsedRoute{map.value(QStringLiteral("route")).toString(), map.value(QStringLiteral("data")), copy, false, nullptr};
            }
        }
    } else {
        ret << parseRoute(values);
    }
    return ret;
}

PageRouter::PageRouter(QQuickItem *parent)
    : QObject(parent)
    , m_paramMap(new QQmlPropertyMap)
    , m_cache()
    , m_preload()
{
    connect(this, &PageRouter::pageStackChanged, [=]() {
        connect(m_pageStack, &ColumnView::currentIndexChanged, this, &PageRouter::currentIndexChanged);
    });
}

QQmlListProperty<PageRoute> PageRouter::routes()
{
    return QQmlListProperty<PageRoute>(this, nullptr, appendRoute, routeCount, route, clearRoutes);
}

void PageRouter::appendRoute(QQmlListProperty<PageRoute> *prop, PageRoute *route)
{
    auto router = qobject_cast<PageRouter *>(prop->object);
    router->m_routes.append(route);
}

int PageRouter::routeCount(QQmlListProperty<PageRoute> *prop)
{
    auto router = qobject_cast<PageRouter *>(prop->object);
    return router->m_routes.length();
}

PageRoute *PageRouter::route(QQmlListProperty<PageRoute> *prop, int index)
{
    auto router = qobject_cast<PageRouter *>(prop->object);
    return router->m_routes[index];
}

void PageRouter::clearRoutes(QQmlListProperty<PageRoute> *prop)
{
    auto router = qobject_cast<PageRouter *>(prop->object);
    router->m_routes.clear();
}

PageRouter::~PageRouter()
{
}

void PageRouter::classBegin()
{
}

void PageRouter::componentComplete()
{
    if (m_pageStack == nullptr) {
        qCCritical(KirigamiLog)
            << "PageRouter should be created with a ColumnView. Not doing so is undefined behaviour, and is likely to result in a crash upon further "
               "interaction.";
    } else {
        Q_EMIT pageStackChanged();
        m_currentRoutes.clear();
        push(parseRoute(initialRoute()));
    }
}

bool PageRouter::routesContainsKey(const QString &key) const
{
    for (auto route : m_routes) {
        if (route->name() == key) {
            return true;
        }
    }
    return false;
}

QQmlComponent *PageRouter::routesValueForKey(const QString &key) const
{
    for (auto route : m_routes) {
        if (route->name() == key) {
            return route->component();
        }
    }
    return nullptr;
}

bool PageRouter::routesCacheForKey(const QString &key) const
{
    for (auto route : m_routes) {
        if (route->name() == key) {
            return route->cache();
        }
    }
    return false;
}

int PageRouter::routesCostForKey(const QString &key) const
{
    for (auto route : m_routes) {
        if (route->name() == key) {
            return route->cost();
        }
    }
    return -1;
}

// It would be nice if this could surgically update the
// param map instead of doing this brute force approach,
// but this seems to work well enough, and prematurely
// optimising stuff is pretty bad if it isn't found as
// a performance bottleneck.
void PageRouter::reevaluateParamMapProperties()
{
    QStringList currentKeys;

    for (auto item : m_currentRoutes) {
        for (auto key : item->properties.keys()) {
            currentKeys << key;

            auto &value = item->properties[key];
            m_paramMap->insert(key, value);
        }
    }

    for (auto key : m_paramMap->keys()) {
        if (!currentKeys.contains(key)) {
            m_paramMap->clear(key);
        }
    }
}

void PageRouter::push(ParsedRoute *route)
{
    Q_ASSERT(route);
    if (!routesContainsKey(route->name)) {
        qCCritical(KirigamiLog) << "Route" << route->name << "not defined";
        return;
    }
    if (routesCacheForKey(route->name)) {
        auto push = [route, this](ParsedRoute *item) {
            m_currentRoutes << item;

            for (auto it = route->properties.begin(); it != route->properties.end(); it++) {
                item->item->setProperty(qUtf8Printable(it.key()), it.value());
                item->properties[it.key()] = it.value();
            }
            reevaluateParamMapProperties();

            m_pageStack->addItem(item->item);
        };
        auto item = m_cache.take(qMakePair(route->name, route->hash()));
        if (item && item->item) {
            push(item);
            return;
        }
        item = m_preload.take(qMakePair(route->name, route->hash()));
        if (item && item->item) {
            push(item);
            return;
        }
    }
    auto context = qmlContext(this);
    auto component = routesValueForKey(route->name);
    auto createAndPush = [component, context, route, this]() {
        // We use beginCreate and completeCreate to allow
        // for a PageRouterAttached to find its parent
        // on construction time.
        auto item = component->beginCreate(context);
        if (item == nullptr) {
            return;
        }
        item->setParent(this);
        auto qqItem = qobject_cast<QQuickItem *>(item);
        if (!qqItem) {
            qCCritical(KirigamiLog) << "Route" << route->name << "is not an item! This is undefined behaviour and will likely crash your application.";
        }
        for (auto it = route->properties.begin(); it != route->properties.end(); it++) {
            qqItem->setProperty(qUtf8Printable(it.key()), it.value());
        }
        route->setItem(qqItem);
        route->cache = routesCacheForKey(route->name);
        m_currentRoutes << route;
        reevaluateParamMapProperties();

        auto attached = qobject_cast<PageRouterAttached *>(qmlAttachedPropertiesObject<PageRouter>(item, true));
        attached->m_router = this;
        component->completeCreate();
        m_pageStack->addItem(qqItem);
        m_pageStack->setCurrentIndex(m_currentRoutes.length() - 1);
    };

    if (component->status() == QQmlComponent::Ready) {
        createAndPush();
    } else if (component->status() == QQmlComponent::Loading) {
        connect(component, &QQmlComponent::statusChanged, [=](QQmlComponent::Status status) {
            // Loading can only go to Ready or Error.
            if (status != QQmlComponent::Ready) {
                qCCritical(KirigamiLog) << "Failed to push route:" << component->errors();
            }
            createAndPush();
        });
    } else {
        qCCritical(KirigamiLog) << "Failed to push route:" << component->errors();
    }
}

QJSValue PageRouter::initialRoute() const
{
    return m_initialRoute;
}

void PageRouter::setInitialRoute(QJSValue value)
{
    m_initialRoute = value;
}

void PageRouter::navigateToRoute(QJSValue route)
{
    auto incomingRoutes = parseRoutes(route);
    QList<ParsedRoute *> resolvedRoutes;

    if (incomingRoutes.length() <= m_currentRoutes.length()) {
        resolvedRoutes = m_currentRoutes.mid(0, incomingRoutes.length());
    } else {
        resolvedRoutes = m_currentRoutes;
        resolvedRoutes.reserve(incomingRoutes.length() - m_currentRoutes.length());
    }

    for (int i = 0; i < incomingRoutes.length(); i++) {
        auto incoming = incomingRoutes.at(i);
        Q_ASSERT(incoming);
        if (i >= resolvedRoutes.length()) {
            resolvedRoutes.append(incoming);
        } else {
            auto current = resolvedRoutes.value(i);
            Q_ASSERT(current);
            auto props = incoming->properties;
            if (current->name != incoming->name || current->data != incoming->data) {
                resolvedRoutes.replace(i, incoming);
            }
            resolvedRoutes[i]->properties.clear();
            for (auto it = props.constBegin(); it != props.constEnd(); it++) {
                resolvedRoutes[i]->properties.insert(it.key(), it.value());
            }
        }
    }

    for (const auto &route : std::as_const(m_currentRoutes)) {
        if (!resolvedRoutes.contains(route)) {
            placeInCache(route);
        }
    }

    m_pageStack->clear();
    m_currentRoutes.clear();
    for (auto toPush : std::as_const(resolvedRoutes)) {
        push(toPush);
    }
    reevaluateParamMapProperties();
    Q_EMIT navigationChanged();
}

void PageRouter::bringToView(QJSValue route)
{
    if (route.isNumber()) {
        auto index = route.toNumber();
        m_pageStack->setCurrentIndex(index);
    } else {
        auto parsed = parseRoute(route);
        auto index = 0;
        for (auto currentRoute : std::as_const(m_currentRoutes)) {
            if (currentRoute->name == parsed->name && currentRoute->data == parsed->data) {
                m_pageStack->setCurrentIndex(index);
                return;
            }
            index++;
        }
        qCWarning(KirigamiLog) << "Route" << parsed->name << "with data" << parsed->data << "is not on the current stack of routes.";
    }
}

bool PageRouter::routeActive(QJSValue route)
{
    auto parsed = parseRoutes(route);
    if (parsed.length() > m_currentRoutes.length()) {
        return false;
    }
    for (int i = 0; i < parsed.length(); i++) {
        if (parsed[i]->name != m_currentRoutes[i]->name) {
            return false;
        }
        if (parsed[i]->data.isValid()) {
            if (parsed[i]->data != m_currentRoutes[i]->data) {
                return false;
            }
        }
    }
    return true;
}

void PageRouter::pushRoute(QJSValue route)
{
    push(parseRoute(route));
    Q_EMIT navigationChanged();
}

void PageRouter::popRoute()
{
    m_pageStack->pop(m_currentRoutes.last()->item);
    placeInCache(m_currentRoutes.last());
    m_currentRoutes.removeLast();
    reevaluateParamMapProperties();
    Q_EMIT navigationChanged();
}

QVariant PageRouter::dataFor(QObject *object)
{
    auto pointer = object;
    auto qqiPointer = qobject_cast<QQuickItem *>(object);
    QHash<QQuickItem *, ParsedRoute *> routes;
    for (auto route : std::as_const(m_cache.items)) {
        routes[route->item] = route;
    }
    for (auto route : std::as_const(m_preload.items)) {
        routes[route->item] = route;
    }
    for (auto route : std::as_const(m_currentRoutes)) {
        routes[route->item] = route;
    }
    while (qqiPointer != nullptr) {
        const auto keys = routes.keys();
        for (auto item : keys) {
            if (item == qqiPointer) {
                return routes[item]->data;
            }
        }
        qqiPointer = qqiPointer->parentItem();
    }
    while (pointer != nullptr) {
        const auto keys = routes.keys();
        for (auto item : keys) {
            if (item == pointer) {
                return routes[item]->data;
            }
        }
        pointer = pointer->parent();
    }
    return QVariant();
}

bool PageRouter::isActive(QObject *object)
{
    auto pointer = object;
    while (pointer != nullptr) {
        auto index = 0;
        for (auto route : std::as_const(m_currentRoutes)) {
            if (route->item == pointer) {
                return m_pageStack->currentIndex() == index;
            }
            index++;
        }
        pointer = pointer->parent();
    }
    qCWarning(KirigamiLog) << "Object" << object << "not in current routes";
    return false;
}

PageRouterAttached *PageRouter::qmlAttachedProperties(QObject *object)
{
    auto attached = new PageRouterAttached(object);
    return attached;
}

QSet<QObject *> flatParentTree(QObject *object)
{
    // See below comment in Climber::climbObjectParents for why this is here.
    static const QMetaObject *metaObject = QMetaType::metaObjectForType(QMetaType::type("QQuickItem*"));
    QSet<QObject *> ret;
    // Use an inline struct type so that climbItemParents and climbObjectParents
    // can call each other
    struct Climber {
        void climbItemParents(QSet<QObject *> &out, QQuickItem *item)
        {
            auto parent = item->parentItem();
            while (parent != nullptr) {
                out << parent;
                climbObjectParents(out, parent);
                parent = parent->parentItem();
            }
        }
        void climbObjectParents(QSet<QObject *> &out, QObject *object)
        {
            auto parent = object->parent();
            while (parent != nullptr) {
                out << parent;
                // We manually call metaObject()->inherits() and
                // use a reinterpret cast because qobject_cast seems
                // to have stability issues here due to mutable
                // pointer mechanics.
                if (parent->metaObject()->inherits(metaObject)) {
                    climbItemParents(out, reinterpret_cast<QQuickItem *>(parent));
                }
                parent = parent->parent();
            }
        }
    };
    Climber climber;
    if (qobject_cast<QQuickItem *>(object)) {
        climber.climbItemParents(ret, qobject_cast<QQuickItem *>(object));
    }
    climber.climbObjectParents(ret, object);
    return ret;
}

void PageRouter::preload(ParsedRoute *route)
{
    for (auto preloaded : std::as_const(m_preload.items)) {
        if (preloaded->equals(route)) {
            delete route;
            return;
        }
    }
    if (!routesContainsKey(route->name)) {
        qCCritical(KirigamiLog) << "Route" << route->name << "not defined";
        delete route;
        return;
    }
    auto context = qmlContext(this);
    auto component = routesValueForKey(route->name);
    auto createAndCache = [component, context, route, this]() {
        auto item = component->beginCreate(context);
        item->setParent(this);
        auto qqItem = qobject_cast<QQuickItem *>(item);
        if (!qqItem) {
            qCCritical(KirigamiLog) << "Route" << route->name << "is not an item! This is undefined behaviour and will likely crash your application.";
        }
        for (auto it = route->properties.begin(); it != route->properties.end(); it++) {
            qqItem->setProperty(qUtf8Printable(it.key()), it.value());
        }
        route->setItem(qqItem);
        route->cache = routesCacheForKey(route->name);
        auto attached = qobject_cast<PageRouterAttached *>(qmlAttachedPropertiesObject<PageRouter>(item, true));
        attached->m_router = this;
        component->completeCreate();
        if (!route->cache) {
            qCCritical(KirigamiLog) << "Route" << route->name << "is being preloaded despite it not having caching enabled.";
            delete route;
            return;
        }
        auto string = route->name;
        auto hash = route->hash();
        m_preload.insert(qMakePair(string, hash), route, routesCostForKey(route->name));
    };

    if (component->status() == QQmlComponent::Ready) {
        createAndCache();
    } else if (component->status() == QQmlComponent::Loading) {
        connect(component, &QQmlComponent::statusChanged, [=](QQmlComponent::Status status) {
            // Loading can only go to Ready or Error.
            if (status != QQmlComponent::Ready) {
                qCCritical(KirigamiLog) << "Failed to push route:" << component->errors();
            }
            createAndCache();
        });
    } else {
        qCCritical(KirigamiLog) << "Failed to push route:" << component->errors();
    }
}

void PageRouter::unpreload(ParsedRoute *route)
{
    ParsedRoute *toDelete = nullptr;
    for (auto preloaded : std::as_const(m_preload.items)) {
        if (preloaded->equals(route)) {
            toDelete = preloaded;
        }
    }
    if (toDelete != nullptr) {
        m_preload.take(qMakePair(toDelete->name, toDelete->hash()));
        delete toDelete;
    }
    delete route;
}

void PreloadRouteGroup::handleChange()
{
    if (!(m_parent->m_router)) {
        qCCritical(KirigamiLog) << "PreloadRouteGroup does not have a parent PageRouter";
        return;
    }
    auto r = m_parent->m_router;
    auto parsed = parseRoute(m_route);
    if (m_when) {
        r->preload(parsed);
    } else {
        r->unpreload(parsed);
    }
}

PreloadRouteGroup::~PreloadRouteGroup()
{
    if (m_parent->m_router) {
        m_parent->m_router->unpreload(parseRoute(m_route));
    }
}

void PageRouterAttached::findParent()
{
    QQuickItem *parent = qobject_cast<QQuickItem *>(this->parent());
    while (parent != nullptr) {
        auto attached = qobject_cast<PageRouterAttached *>(qmlAttachedPropertiesObject<PageRouter>(parent, false));
        if (attached != nullptr && attached->m_router != nullptr) {
            m_router = attached->m_router;
            Q_EMIT routerChanged();
            Q_EMIT dataChanged();
            Q_EMIT isCurrentChanged();
            Q_EMIT navigationChanged();
            break;
        }
        parent = parent->parentItem();
    }
}

void PageRouterAttached::navigateToRoute(QJSValue route)
{
    if (m_router) {
        m_router->navigateToRoute(route);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return;
    }
}

bool PageRouterAttached::routeActive(QJSValue route)
{
    if (m_router) {
        return m_router->routeActive(route);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return false;
    }
}

void PageRouterAttached::pushRoute(QJSValue route)
{
    if (m_router) {
        m_router->pushRoute(route);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return;
    }
}

void PageRouterAttached::popRoute()
{
    if (m_router) {
        m_router->popRoute();
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return;
    }
}

void PageRouterAttached::bringToView(QJSValue route)
{
    if (m_router) {
        m_router->bringToView(route);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return;
    }
}

QVariant PageRouterAttached::data() const
{
    if (m_router) {
        return m_router->dataFor(parent());
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return QVariant();
    }
}

bool PageRouterAttached::isCurrent() const
{
    if (m_router) {
        return m_router->isActive(parent());
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return false;
    }
}

bool PageRouterAttached::watchedRouteActive()
{
    if (m_router) {
        return m_router->routeActive(m_watchedRoute);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
        return false;
    }
}

void PageRouterAttached::setWatchedRoute(QJSValue route)
{
    m_watchedRoute = route;
    Q_EMIT watchedRouteChanged();
}

QJSValue PageRouterAttached::watchedRoute()
{
    return m_watchedRoute;
}

void PageRouterAttached::pushFromHere(QJSValue route)
{
    if (m_router) {
        m_router->pushFromObject(parent(), route);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
    }
}

void PageRouterAttached::replaceFromHere(QJSValue route)
{
    if (m_router) {
        m_router->pushFromObject(parent(), route, true);
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
    }
}

void PageRouterAttached::popFromHere()
{
    if (m_router) {
        m_router->pushFromObject(parent(), QJSValue());
    } else {
        qCCritical(KirigamiLog) << "PageRouterAttached does not have a parent PageRouter";
    }
}

void PageRouter::placeInCache(ParsedRoute *route)
{
    Q_ASSERT(route);
    if (!route->cache) {
        delete route;
        return;
    }
    auto string = route->name;
    auto hash = route->hash();
    m_cache.insert(qMakePair(string, hash), route, routesCostForKey(route->name));
}

void PageRouter::pushFromObject(QObject *object, QJSValue inputRoute, bool replace)
{
    const auto parsed = parseRoutes(inputRoute);
    const auto objects = flatParentTree(object);

    for (const auto &obj : objects) {
        bool popping = false;
        for (auto route : std::as_const(m_currentRoutes)) {
            if (popping) {
                m_currentRoutes.removeAll(route);
                reevaluateParamMapProperties();
                placeInCache(route);
                continue;
            }
            if (route->item == obj) {
                m_pageStack->pop(route->item);
                if (replace) {
                    m_currentRoutes.removeAll(route);
                    reevaluateParamMapProperties();
                    m_pageStack->removeItem(route->item);
                }
                popping = true;
            }
        }
        if (popping) {
            if (!inputRoute.isUndefined()) {
                for (auto route : parsed) {
                    push(route);
                }
            }
            Q_EMIT navigationChanged();
            return;
        }
    }
    qCWarning(KirigamiLog) << "Object" << object << "not in current routes";
}

QJSValue PageRouter::currentRoutes() const
{
    auto engine = qjsEngine(this);
    auto ret = engine->newArray(m_currentRoutes.length());
    for (int i = 0; i < m_currentRoutes.length(); ++i) {
        auto object = engine->newObject();
        object.setProperty(QStringLiteral("route"), m_currentRoutes[i]->name);
        object.setProperty(QStringLiteral("data"), engine->toScriptValue(m_currentRoutes[i]->data));
        auto keys = m_currentRoutes[i]->properties.keys();
        for (auto key : keys) {
            object.setProperty(key, engine->toScriptValue(m_currentRoutes[i]->properties[key]));
        }
        ret.setProperty(i, object);
    }
    return ret;
}

PageRouterAttached::PageRouterAttached(QObject *parent)
    : QObject(parent)
    , m_preload(new PreloadRouteGroup(this))
{
    findParent();
    auto item = qobject_cast<QQuickItem *>(parent);
    if (item != nullptr) {
        connect(item, &QQuickItem::windowChanged, this, [this]() {
            findParent();
        });
        connect(item, &QQuickItem::parentChanged, this, [this]() {
            findParent();
        });
    }
}
