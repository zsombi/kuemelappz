/**
  PageStack - element that makes possible to navigate between pages. The visited
  pages are kept in a stak in memory and are discarded once they are navigated backwards
  out.
  */
import QtQuick 1.0
import "." 1.0
import "Utility.js" as Utils

Item {
    id: root
    // page stack depth, aka number of pages in the stack
    property int depth: privates.stackDepth()
    // points to teh current page
    property Item currentPage: null
    // fading duration and easing: themed, but
    property int pageFadingDuration: THEME.pageFadingDuration
    property int pageFadingEasing: THEME.pageFadingEasing

    // bind system busy state with stack's busy state
    Binding {
        target: widgetSet
        property: "busy"
        value: privates.busy
    }

    // no anchoring!!
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    function push(page, properties) {
        return privates.push(page, properties)
    }

    function pop(page) {
        return privates.pop(page)
    }
    function back()
    {
        var pageStack = Utils.getPgStack()
        if (pageStack.length > 1)
            return pop()
    }

    function clear() {
        return privates.clear();
    }

    /**
      Privates
      */

    QtObject {
        id: privates
        property int ongoingTransitionCount: 0
        property bool busy: ongoingTransitionCount > 0
        function stackDepth()
        {
            //return (pageStack !== undefined) ? pageStack.length : 0
            return Utils.getPgStack().length
        }

        function initializePage(page, properties)
        {
            var component = pageHolder.createObject(root)
            var pgComponent;
            if (page.createObject != undefined) {
                pgComponent = page
            } else if (typeof page == "string") {
                pgComponent = Qt.createComponent(page)
            }
            if (pgComponent) {
                if (pgComponent.status == Component.Error)
                    throw new Error("Error while loading page: " + pageComp.errorString());
                else {
                    page = pgComponent.createObject(component, properties || {})
                }
            }
            else
                throw new Error("Unknown page given:" + page);

            component.page = page;
            component.owner = page.parent;

            // the page has to be reparented if
            if (page.parent != component) {
                page.parent = component;
            }

            if (page.pageStack !== undefined) {
                page.pageStack = root;
            }

            return component;
        }

        function push(page, properties)
        {
            if (page == currentPage)
                throw new Error("Cannot push current page twice!")
            var pageStack = Utils.getPgStack()
            var oldContainer = pageStack[pageStack.length - 1]

            var container = initializePage(page, properties)

            pageStack.push(container)

            depth = pageStack.length
            currentPage = container.page
            if (oldContainer)
                oldContainer.pushExit()
            container.pushEnter()
            return container.page
        }

        function pop(page)
        {
            var pageStack = Utils.getPgStack()
            if (pageStack.length < 1)
                return null

            var oldContainer = pageStack.pop()
            var container = pageStack[pageStack.length - 1]
            if (page !== undefined) {
                // page has been spoecified, unwind till we get it
                while ((page != container.page) && (pgStack.length > 1)) {
                    container.cleanup();
                    pageStack.pop();
                    container = pageStack[pageStack.length - 1];
                }
            }
            depth = pageStack.length
            currentPage = container.page
            oldContainer.popExit()
            container.popEnter()
            return oldContainer.page
        }

        function clearStack()
        {
            var pageStack = Utils.getPgStack()
            var container;
            while (container = pageStack.pop()) {
                container.cleanup();
            }
            depth = 0;
            currentPage = null;
        }

        // updates page status by setting intermediate page statuses to provide task splitting
        function setPageStatus(page, status) {
            if (page.status !== undefined) {
                // set intermediate statuses
                if ((status == WidgetSet.PageActive) && (page.status == WidgetSet.PageInactive)) {
                    page.status = WidgetSet.PageActivating;
                } else if ((status == WidgetSet.PageInactive) && (page.status == WidgetSet.PageActive)) {
                    page.status = WidgetSet.PageDeactivating;
                }
                // set the target status
                page.status = status;
            }
        }
    }

    onVisibleChanged: {
        if (currentPage) {
            var tgtStatus = visible ? WidgetSet.PageActive : WidgetSet.PageInactive
            privates.setPageStatus(currentPage, tgtStatus);
        }
    }

    // Page holder - holds pages created upon push operations
    Component {
        id: pageHolder

        Item {
            id: container

            width: parent ? parent.width : 0
            height: parent ? parent.height : 0

            state: "hidden"
            // hosted page.
            property Item page: null
            // page owner.
            property Item owner: null
            // true when pop operation performed to clean component
            property bool destroyUponExit: false

            // Performs a push enter transition.
            function pushEnter() {
                // enter from right side, then reset
                state = 'rightEdge'
                state = '';
                // make page visible
                page.visible = true;
            }

            // Performs a push exit transition.
            function pushExit() {
                // fade out to left then reset
                state = "leftEdge";
            }

            // Performs a pop enter transition.
            function popEnter() {
                // enters from left side
                state = "leftEdge";
                state = "";
                page.visible = true;
            }

            // Performs a pop exit transition.
            function popExit() {
                state = "rightEdge";
                destroyUponExit = true;
            }
            
            // Called when a transition has started.
            function transitionStarted() {
                privates.ongoingTransitionCount++;
                if (root.visible) {
                    privates.setPageStatus(page, (state == "") ? WidgetSet.PageActivating : WidgetSet.PageDeactivating);
                }
            }
            
            // Called when a transition has ended.
            function transitionEnded() {
                if (state != "") {
                    state = "hidden";
                }
                if (root.visible) {
                    privates.setPageStatus(page, (state == "") ? WidgetSet.PageActive : WidgetSet.PageInactive);
                }
                privates.ongoingTransitionCount--;
                if (destroyUponExit) {
                    cleanup();
                }
            }

            // Cleans up the container and then destroys it.
            function cleanup() {
                if (page.status == WidgetSet.PageActive) {
                    privates.setPageStatus(page, WidgetSet.PageInactive);
                }
                if (owner != container) {
                    // container is not the owner of the page - re-parent back to original owner
                    page.visible = false;
                    page.parent = owner;
                }
                container.destroy();
            }

            states: [
                // Explicit properties for default state.
                State {
                    name: ""
                    PropertyChanges { target: container; visible: true }
                },
                // Start state for pop entry, end state for push exit.
                State {
                    name: "leftEdge"
                    PropertyChanges { target: container; x: -width }
                },
                // Start state for push entry, end state for pop exit.
                State {
                    name: "rightEdge"
                    PropertyChanges { target: container; x: width }
                },
                // Inactive state.
                State {
                    name: "hidden"
                    PropertyChanges { target: container; visible: false }
                }
            ]

            transitions: [
                // right-to-left transition
                Transition {
                    from: ""; to: "leftEdge"
                    reversible: true
                    SequentialAnimation {
                        ScriptAction {
                            script:
                                if (state == "leftEdge")
                                    transitionStarted()
                                else
                                    transitionEnded()
                        }
                        PropertyAnimation { properties: "x"; easing.type: pageFadingEasing; duration: pageFadingDuration }
                        ScriptAction {
                            script:
                                if (state == "leftEdge")
                                    transitionEnded()
                                else
                                    transitionStarted()
                        }
                    }
                },
                // left-to-right transition
                Transition {
                    from: ""; to: "rightEdge"
                    reversible: true
                    SequentialAnimation {
                        ScriptAction {
                            script:
                                if (state == "rightEdge")
                                    transitionStarted()
                                else
                                    transitionEnded()
                        }
                        PropertyAnimation { properties: "x"; easing.type: pageFadingEasing; duration: pageFadingDuration }
                        ScriptAction {
                            script:
                                if (state == "rightEdge")
                                    transitionEnded()
                                else
                                    transitionStarted()
                        }
                    }
                }
            ]
        }
    }

}

