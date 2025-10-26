import HostEnvironment from './hostEnvironment.js';
import {
  DiscreteEventPriority,
  ContinuousEventPriority,
  DefaultEventPriority,
} from 'react-reconciler/constants';

// This implements all required host config functions for React Reconciler

// Priority constants (matching React's internal values)
const ImmediatePriority = 1;
const UserBlockingPriority = 2;
const NormalPriority = 3;
const LowPriority = 4;
const IdlePriority = 5;

// Lane constants (React's lane-based scheduling)
const SyncLane = 1;
const InputContinuousLane = 2;
const DefaultLane = 4;
const TransitionLane = 8;
const RetryLane = 16;
const IdleLane = 32;
const OffscreenLane = 64;

// Current update priority state
let currentUpdatePriority = NormalPriority;


// Host config object
// See reference https://github.com/facebook/react/tree/main/packages/react-reconciler
const ReactHostConfig = {

// ===== Core Methods =====

/**
 * Creates a host instance (DOM element in web, native view in mobile, custom object in V8)
 * @param {string} type - The type of instance to create
 * @param {Object} props - The props for the instance
 * @param {Object} rootContainerInstance - The root container
 * @param {Object} hostContext - The host context
 * @returns {Object} The created instance
 */
createInstance: function(type, props, rootContainerInstance, hostContext) {
    new Journal().log("react-reconciler::createInstance");
    const node = HostEnvironment.createNode(type);
    
    // 处理 props
    Object.keys(props).forEach((key) => {
        // return directly when shouldSetTextContent return true
        if (key === "children") return;
        
        if (key.startsWith("on") && typeof props[key] === "function") {
            // 处理事件
            HostEnvironment.addEventListener(node, key, props[key]);
        } else if (key === "style") {
            // 处理样式
            Object.entries(props[key]).forEach(([styleKey, value]) => {
                HostEnvironment.setProperty(node, styleKey, value);
            });
        } else {
            // 处理其他属性
            HostEnvironment.setProperty(node, key, props[key]);
        }
    });
    
    return node;
},

/**
 * Creates a text instance
 * @param {string} text - The text content
 * @param {Object} rootContainerInstance - The root container
 * @param {Object} hostContext - The host context
 * @returns {Object} The text instance
 */
createTextInstance: function(text, rootContainerInstance, hostContext) {
    new Journal().log("react-reconciler::createTextInstance");
    return HostEnvironment.createTextNode(text);
},

/**
 * Some target platforms support setting an instance's text content without manually creating a text node.
 * If you return true from this method, React will assume that this node's children are text, and will not create nodes for them. 
 * You will need to implement resetTextContent too.
 * @param {string} type - The type
 * @returns {boolean} Whether to set text content
 */
shouldSetTextContent: function(type) {
    new Journal().log("react-reconciler::shouldSetTextContent");
    return false;
},

/**
 * Appends a child to a parent instance
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to append
 */
appendInitialChild: function(parent, child) {
    new Journal().log("react-reconciler::appendInitialChild");
    HostEnvironment.addNode(parent, child);
},

/**
 * Finalizes initial children.
 * If you don't want to do anything here, you should return false.
 * @param {Object} instance - The instance
 * @param {string} type - The type
 * @param {Object} props - The props
 * @param {Object} hostContext - The host context
 * @returns {boolean} Whether the instance needs updates
 */
finalizeInitialChildren: function(instance, type, props, hostContext) {
    new Journal().log("react-reconciler::finalizeInitialChildren");
    return false;
},

/**
 * Gets the root host context.
 * If you don't intend to use host context, you can return null.
 * @param {Object} rootContainerInstance - The root container
 * @returns {Object} The root host context
 */
getRootHostContext: function(rootContainerInstance) {
    new Journal().log("react-reconciler::getRootHostContext");
    return {};
},

/**
 * Gets the child host context.
 * If you don't want to do anything here, return parentHostContext.
 * @param {Object} parentHostContext - The parent host context
 * @param {string} type - The type
 * @param {Object} rootContainerInstance - The root container
 * @returns {Object} The child host context
 */
getChildHostContext: function(parentHostContext, type, rootContainerInstance) {
    new Journal().log("react-reconciler::getChildHostContext");
    return parentHostContext;
},

// If you don't want to do anything here, return instance.
getPublicInstance: function(instance){
    return instance;
},

/**
 * Prepares for commit
 * This method lets you store some information before React starts making changes to the tree on the screen. 
 * For example, the DOM renderer stores the current text selection so that it can later restore it. This method is mirrored by resetAfterCommit.
 * Even if you don't want to do anything here, you need to return null from it.
 * @param {Object} rootContainerInstance - The root container
 */
prepareForCommit: function(rootContainerInstance) {
    new Journal().log("react-reconciler::prepareForCommit");
    return {};
},

/**
 * Resets after commit
 * This method is called right after React has performed the tree mutations. You can use it to restore something you've stored in prepareForCommit — for example, text selection.
 * You can leave it empty.
 * @param {Object} rootContainerInstance - The root container
 */
resetAfterCommit: function(rootContainerInstance) {
    new Journal().log("react-reconciler::resetAfterCommit");
},

/**
 * Schedules a timeout
 * You can proxy this to setTimeout or its equivalent in your environment.
 * @param {Function} callback - The callback to execute
 * @param {number} timeout - The timeout in milliseconds
 * @returns {number} The timeout ID
 */
scheduleTimeout: function(callback, timeout) {
    new Journal().log("react-reconciler::scheduleTimeout");
    return setTimeout(callback, timeout);
},

/**
 * Cancels a timeout
 * You can proxy this to clearTimeout or its equivalent in your environment.
 * @param {number} timeoutID - The timeout ID to cancel
 */
cancelTimeout: function(timeoutID) {
    new Journal().log("react-reconciler::cancelTimeout");
    clearTimeout(timeoutID);
},

/**
 * No timeout value
 * This is a property (not a function) that should be set to something that can never be a valid timeout ID. For example, you can set it to -1.
 */
noTimeout: -1,

/**
 * Whether microtasks are supported
 * Set this to true to indicate that your renderer supports scheduleMicrotask. 
 * We use microtasks as part of our discrete event implementation in React DOM. 
 * If you're not sure if your renderer should support this, you probably should. 
 * The option to not implement scheduleMicrotask exists so that platforms with more control over user events, 
 * like React Native, can choose to use a different mechanism.
 */
supportsMicrotasks: true,

/**
 * Schedules a microtask
 * Optional. You can proxy this to queueMicrotask or its equivalent in your environment.
 * @param {Function} callback - The callback to execute
 */
scheduleMicrotask: function(callback) {
    new Journal().log("react-reconciler::scheduleMicrotask");
    // For V8 engine, you might use queueMicrotask or Promise.resolve().then()
    if (typeof queueMicrotask === 'function') {
        queueMicrotask(callback);
    } else {
        Promise.resolve().then(callback);
    }
},

/**
 * Whether this is the primary renderer.
 * This is a property (not a function) that should be set to true if your renderer is the main one on the page. 
 * For example, if you're writing a renderer for the Terminal, it makes sense to set it to true, 
 * but if your renderer is used on top of React DOM or some other existing renderer, set it to false.
 */
isPrimaryRenderer: true,

/**
 * Whether the renderer supports mutation
 */
supportsMutation: true,

/**
 * Whether the renderer supports persistence
 */
supportsPersistence: false,

// ===== Mutation Methods =====
// Note: If you're using React in mutation mode (you probably do), you'll need to implement a few more methods!!!
// The reconciler has two modes: mutation mode and persistent mode. You must specify one of them.
// If your target platform is similar to the DOM and has methods similar to appendChild, removeChild, and so on, you'll want to use the mutation mode. 
// This is the same mode used by React DOM, React ART, and the classic React Native renderer.
// If your target platform has immutable trees, you'll want the persistent mode instead. 
// In that mode, existing nodes are never mutated, and instead every change clones the parent tree and then replaces the whole parent tree at the root. 
// This is the mode used by the new React Native renderer, codenamed "Fabric".
// If you're not sure which one you want, you likely need the mutation mode.

/**
 * Appends a child to a container
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to append
 */
appendChild: function(parentInstance, child) {
    new Journal().log("react-reconciler::appendChild");
    HostEnvironment.addNode(parentInstance, child);
},

/**
 * Appends a child to a container
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to append
 */
appendChildToContainer: function(parentInstance, child) {
    new Journal().log("react-reconciler::appendChildToContainer");
    HostEnvironment.addNode(parentInstance, child);
},

/**
 * Inserts a child before another child
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to insert
 * @param {Object} beforeChild - The child to insert before
 */
insertBefore: function(parentInstance, child, beforeChild) {
    new Journal().log("react-reconciler::insertBefore");
    // TODO: implement a "real" insertBefore
    HostEnvironment.addNode(parentInstance, child);
},

/**
 * Inserts a child before another child in a container
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to insert
 * @param {Object} beforeChild - The child to insert before
 */
insertInContainerBefore: function(parentInstance, child, beforeChild) {
    new Journal().log("react-reconciler::insertInContainerBefore");
    // TODO: implement a "real" insertBefore
    HostEnvironment.addNode(parentInstance, child);
},

/**
 * Removes a child
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to remove
 */
removeChild: function(parentInstance, child) {
    new Journal().log("react-reconciler::removeChild");
    HostEnvironment.removeNode(parentInstance, child);
},

/**
 * Removes a child from a container
 * @param {Object} parentInstance - The parent instance
 * @param {Object} child - The child to remove
 */
removeChildFromContainer: function(parentInstance, child) {
    new Journal().log("react-reconciler::removeChildFromContainer");
    HostEnvironment.removeNode(parentInstance, child);
},

// If you returned true from shouldSetTextContent for the previous props, but returned false from shouldSetTextContent for the next props, 
// React will call this method so that you can clear the text content you were managing manually. 
// If you never return true from shouldSetTextContent, you can leave it empty.
resetTextContent: function(instance) {
    new Journal().log("react-reconciler::resetTextContent");
},

/**
 * Commits text updates。
 * @param {Object} textInstance - The text instance is a node created by createTextInstance.
 * @param {string} oldText - The old text
 * @param {string} newText - The new text
 */
commitTextUpdate: function(textInstance, oldText, newText) {
    new Journal().log("react-reconciler::commitTextUpdate");
    if (oldText === newText) 
        return;
    HostEnvironment.setText(textInstance, newText);
},

/**
 * Commits mount operations
 * This method is only called if you returned true from finalizeInitialChildren for this instance.
 * @param {Object} instance - The instance to mount
 * @param {string} type - The type
 * @param {Object} newProps - The new props
 * @param {Object} rootContainerInstance - The root container
 */
commitMount: function(instance, type, newProps, rootContainerInstance) {
    new Journal().log("react-reconciler::commitMount");
},

/**
 * Prepares an update for an instance
 * This method compares oldProps and newProps to determine what needs to be updated.
 * @param {Object} instance - The instance to update
 * @param {string} type - The type
 * @param {Object} oldProps - The old props
 * @param {Object} newProps - The new props
 * @param {Object} rootContainerInstance - The root container
 * @param {Object} hostContext - The host context
 * @returns {Object|null} Update payload or null if no updates needed
 */
prepareUpdate: function(instance, type, oldProps, newProps, rootContainerInstance, hostContext) {
    new Journal().log("react-reconciler::prepareUpdate");
    
    const updatePayload = [];
    let hasUpdates = false;
    
    // 比较属性变化
    const allKeys = new Set([...Object.keys(oldProps), ...Object.keys(newProps)]);
    
    allKeys.forEach((key) => {
        if (key === "children") return;
        
        const oldValue = oldProps[key];
        const newValue = newProps[key];
        
        if (oldValue !== newValue) {
            updatePayload.push(key, newValue);
            hasUpdates = true;
        }
    });
    
    // 如果有更新，返回更新载荷；否则返回 null
    return hasUpdates ? updatePayload : null;
},

/**
 * Commits updates to an instance
 * This method should mutate the instance to match nextProps.
 * @param {Object} instance - The instance to update
 * @param {string} type - The type
 * @param {Object} prevProps - The previous props
 * @param {Object} nextProps - The next props
 * @param {Object} internalHandle - The internal handle
 */
commitUpdate: function(instance, type, prevProps, nextProps, internalHandle) {
    new Journal().log("react-reconciler::commitUpdate");
    
    // 更新属性
    Object.keys(nextProps).forEach((key) => {
        if (key === "children") return;
        
        const oldValue = prevProps[key];
        const newValue = nextProps[key];
        
        if (oldValue !== newValue) {
            if (key.startsWith("on") && typeof newValue === "function") {
                // 移除旧的事件监听器
                if (oldValue && typeof oldValue === "function") {
                    HostEnvironment.removeEventListener(instance, key, oldValue);
                }
                // 添加新的事件监听器
                HostEnvironment.addEventListener(instance, key, newValue);
            } else if (key.startsWith("on") && !newValue) {
                // 如果新值为空，移除事件监听器
                if (oldValue && typeof oldValue === "function") {
                    HostEnvironment.removeEventListener(instance, key, oldValue);
                }
            } else if (key === "style") {
                Object.entries(newValue).forEach(([styleKey, value]) => {
                    HostEnvironment.setProperty(instance, styleKey, value);
                });
            } else {
                HostEnvironment.setProperty(instance, key, newValue);
            }
        }
    });
},

// This method should make the instance invisible without removing it from the tree. 
// For example, it can apply visual styling to hide it. It is used by Suspense to hide the tree while the fallback is visible.
hideInstance: function(instance){
    // TODO: This function can be used to hide window???
},

// Same as hideInstance, but for nodes created by createTextInstance.
hideTextInstance: function(textInstance){

},

// This method should make the instance visible, undoing what hideInstance did.
unhideInstance: function(instance, props){

},

// Same as unhideInstance, but for nodes created by createTextInstance.
unhideTextInstance: function(textInstance, text){

},


// ===== PRIORITY MANAGEMENT =====

getCurrentEventPriority: function() {
    return DefaultEventPriority;
},

/**
 * Gets the current update priority
 * @returns {number} The current priority level
 */
getCurrentUpdatePriority: function() {
    return currentUpdatePriority;
},

/**
 * Sets the current update priority
 * @returns {number} The current priority level
 */
setCurrentUpdatePriority: function(currentUpdatePriority) {
},

/**
 * Resolves the current update priority to a lane
 * @returns {number} A lane value representing the current priority
 */
resolveUpdatePriority: function() {
    // Map priority levels to appropriate lanes
    switch (currentUpdatePriority) {
    case ImmediatePriority:
        return SyncLane; // Highest priority - synchronous
    case UserBlockingPriority:
        return InputContinuousLane; // User interactions
    case NormalPriority:
        return DefaultLane; // Normal updates
    case LowPriority:
        return TransitionLane; // Low priority updates
    case IdlePriority:
        return IdleLane; // Idle time updates
    default:
        return DefaultLane; // Fallback to normal priority
    }
},

/**
 * Whether the renderer supports hydration
 */
supportsHydration: false,

/**
 * Not pending transition
 */
NotPendingTransition: {},

/**
 * Host transition context
 */
HostTransitionContext: {},

// ===== TEST SELECTORS =====

/**
 * Whether test selectors are supported
 */
supportsTestSelectors: false,

// ===== CONTAINER OPERATIONS =====

/**
 * Clears a container
 * @param {Object} container - The container to clear
 */
clearContainer: function(container) {
    new Journal().log("clearContainer");
    if (container.children) {
    container.children = [];
    }
},

// ===== CONTAINER CHILD SET OPERATIONS =====

/**
 * Creates a container child set
 * @returns {Object} The container child set
 */
createContainerChildSet: function() {
    new Journal().log("createContainerChildSet");
    return [];
},

// ===== SUSPENSE SUPPORT =====

/**
 * Whether commit may suspend
 */
maySuspendCommit: function() {
    new Journal().log("maySuspendCommit");
    return false;
},

// ===== RENDERER METADATA =====

/**
 * Renderer version
 */
rendererVersion: '1.0.0',

/**
 * Renderer package name
 */
rendererPackageName: 'morph',

/**
 * Extra dev tools config
 */
extraDevToolsConfig: null
};

import ReactReconciler from 'react-reconciler';

const reconciler = ReactReconciler(ReactHostConfig);

const CustomRenderer = {
    render(element, containerNode, callback) {
        console.log("CustomRenderer::render");
        const container = reconciler.createContainer(containerNode, false, false);
        reconciler.updateContainer(element, container, null, callback);
    }
};

export default CustomRenderer;