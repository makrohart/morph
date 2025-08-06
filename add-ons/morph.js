  const {React, ReactDOM, Scheduler, ReactReconciler} = Reacts;

  const HostEnvironment = {
    isValidNode: (node) =>{
      return node instanceof MorphNode ||
             node instanceof MorphDivNode ||
             node instanceof MorphButtonNode ||
             node instanceof MorphWindowNode;
    },

    createNode: (type) => {
      new Journal().log("createNode");
      if (type === "button")
        return new MorphButtonNode();
      else if (type === "div")
        return new MorphDivNode();
      else if (type === "window")
        return new MorphWindowNode();
    },

    addNode: (parent, child) => {
      new Journal().log("addNode");
      // TODO: support more node other than MorphNode
      if (HostEnvironment.isValidNode(child) && HostEnvironment.isValidNode(parent))
        parent.add(child);
    },

    removeNode: (parent, child) => {
      new Journal().log("removeNode");
      // TODO: support more node other than MorphNode
      if (HostEnvironment.isValidNode(child) && HostEnvironment.isValidNode(parent))
        parent.remove(child);
    },

  }


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
  const ReactHostConfig = {
    // ===== PRIORITY MANAGEMENT =====
    
    /**
     * Gets the current update priority
     * @returns {number} The current priority level
     */
    getCurrentUpdatePriority: function() {
      new Journal().log("getCurrentUpdatePriority");
      return currentUpdatePriority;
    },

    /**
     * Sets the current update priority
     * @returns {number} The current priority level
     */
    setCurrentUpdatePriority: function(currentUpdatePriority) {
      new Journal().log("getCurrentUpdatePriority");
    },

    /**
     * Resolves the current update priority to a lane
     * @returns {number} A lane value representing the current priority
     */
    resolveUpdatePriority: function() {
      new Journal().log("resolveUpdatePriority");
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

    // ===== HOST ENVIRONMENT =====
    
    /**
     * Creates a host instance (DOM element in web, native view in mobile, custom object in V8)
     * @param {string} type - The type of instance to create
     * @param {Object} props - The props for the instance
     * @param {Object} rootContainerInstance - The root container
     * @param {Object} hostContext - The host context
     * @returns {Object} The created instance
     */
    createInstance: function(type, props, rootContainerInstance, hostContext) {
      new Journal().log("createInstance");
      node = HostEnvironment.createNode(type);
      Object.keys(props).forEach((key) => {
        if (key === "children") return;
        if (key.startsWith("on") && typeof props[key] === "function") {
          if (HostEnvironment.isValidNode(node))
              node.on(key, props[key]);
        } else if (key === "style") {
          Object.entries(props[key]).forEach(([styleKey, value]) => {
            if (HostEnvironment.isValidNode(node))
              node.setProperty(styleKey, value);
          });
        }
      });
      return node;
    },

    // 关闭不需要的功能
    /**
     * Determines if text content should be set
     * @param {string} type - The type
     * @returns {boolean} Whether to set text content
     */
    shouldSetTextContent: function(type) {
      new Journal().log("shouldSetTextContent");
      return false;
    },

    /**
     * Creates a text instance
     * @param {string} text - The text content
     * @param {Object} rootContainerInstance - The root container
     * @param {Object} hostContext - The host context
     * @returns {Object} The text instance
     */
    createTextInstance: function(text, rootContainerInstance, hostContext) {
      new Journal().log("createTextInstance");
      return {
        type: 'text',
        text: text,
      };
    },

    /**
     * Appends a child to a parent instance
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to append
     */
    appendInitialChild: function(parent, child) {
      new Journal().log("appendInitialChild");
      HostEnvironment.addNode(parent, child);
    },

    /**
     * Finalizes initial children
     * @param {Object} instance - The instance
     * @param {string} type - The type
     * @param {Object} props - The props
     * @param {Object} hostContext - The host context
     * @returns {boolean} Whether the instance needs updates
     */
    finalizeInitialChildren: function(instance, type, props, hostContext) {
      new Journal().log("finalizeInitialChildren");
      return false; // No updates needed initially
    },

    // ===== MUTATION OPERATIONS =====
    
    /**
     * Appends a child to a container
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to append
     */
    appendChild: function(parentInstance, child) {
      new Journal().log("appendChild");
      HostEnvironment.addNode(parentInstance, child);
    },

    /**
     * Appends a child to a container
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to append
     */
    appendChildToContainer: function(parentInstance, child) {
      new Journal().log("appendChildToContainer");
      HostEnvironment.addNode(parentInstance, child);
    },

    /**
     * Inserts a child before another child
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to insert
     * @param {Object} beforeChild - The child to insert before
     */
    insertBefore: function(parentInstance, child, beforeChild) {
      new Journal().log("insertBefore");
      HostEnvironment.addNode(parentInstance, child);
    },

    /**
     * Inserts a child before another child in a container
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to insert
     * @param {Object} beforeChild - The child to insert before
     */
    insertInContainerBefore: function(parentInstance, child, beforeChild) {
      new Journal().log("insertInContainerBefore");
      HostEnvironment.addNode(parentInstance, child);
    },

    /**
     * Removes a child
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to remove
     */
    removeChild: function(parentInstance, child) {
      new Journal().log("removeChild");
      HostEnvironment.removeNode(parentInstance, child);
    },

    /**
     * Removes a child from a container
     * @param {Object} parentInstance - The parent instance
     * @param {Object} child - The child to remove
     */
    removeChildFromContainer: function(parentInstance, child) {
      new Journal().log("removeChildFromContainer");
      HostEnvironment.removeNode(parentInstance, child);
    },

    /**
     * Commits text updates
     * @param {Object} textInstance - The text instance
     * @param {string} oldText - The old text
     * @param {string} newText - The new text
     */
    commitTextUpdate: function(textInstance, oldText, newText) {
      new Journal().log("commitTextUpdate");
      textInstance.text = newText;
    },

    /**
     * Commits updates to an instance
     * @param {Object} instance - The instance to update
     * @param {string} type - The type
     * @param {Object} oldProps - The old props
     * @param {Object} newProps - The new props
     * @param {Object} rootContainerInstance - The root container
     */
    commitUpdate: function(instance, type, oldProps, newProps, rootContainerInstance) {
      new Journal().log("commitUpdate");
      // TODO: set layout and render
    },

    /**
     * Commits mount operations
     * @param {Object} instance - The instance to mount
     * @param {string} type - The type
     * @param {Object} newProps - The new props
     * @param {Object} rootContainerInstance - The root container
     */
    commitMount: function(instance, type, newProps, rootContainerInstance) {
      new Journal().log("commitMount");
    },

    // ===== CONTEXT MANAGEMENT =====
    
    /**
     * Gets the root host context
     * @param {Object} rootContainerInstance - The root container
     * @returns {Object} The root host context
     */
    getRootHostContext: function(rootContainerInstance) {
      new Journal().log("getRootHostContext");
      return {};
    },

    /**
     * Gets the child host context
     * @param {Object} parentHostContext - The parent host context
     * @param {string} type - The type
     * @param {Object} rootContainerInstance - The root container
     * @returns {Object} The child host context
     */
    getChildHostContext: function(parentHostContext, type, rootContainerInstance) {
      new Journal().log("getChildHostContext");
      return {};
    },

    // ===== PREPARATION AND RESET =====
    
    /**
     * Prepares for commit
     * @param {Object} rootContainerInstance - The root container
     */
    prepareForCommit: function(rootContainerInstance) {
      new Journal().log("prepareForCommit");
      // Prepare for any commit operations
    },

    /**
     * Resets after commit
     * @param {Object} rootContainerInstance - The root container
     */
    resetAfterCommit: function(rootContainerInstance) {
      new Journal().log("resetAfterCommit");
      // Clean up after commit operations
    },

    // ===== TIMEOUT MANAGEMENT =====
    
    /**
     * Schedules a timeout
     * @param {Function} callback - The callback to execute
     * @param {number} timeout - The timeout in milliseconds
     * @returns {number} The timeout ID
     */
    scheduleTimeout: function(callback, timeout) {
      new Journal().log("scheduleTimeout");
      return setTimeout(callback, timeout);
    },

    /**
     * Cancels a timeout
     * @param {number} timeoutID - The timeout ID to cancel
     */
    cancelTimeout: function(timeoutID) {
      new Journal().log("cancelTimeout");
      clearTimeout(timeoutID);
    },

    /**
     * No timeout value
     */
    noTimeout: -1,

    // ===== RENDERER CONFIGURATION =====
    
    /**
     * Whether this is the primary renderer
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

    // ===== MICROTASK SUPPORT =====
    
    /**
     * Whether microtasks are supported
     */
    supportsMicrotasks: true,

    /**
     * Schedules a microtask
     * @param {Function} callback - The callback to execute
     */
    scheduleMicrotask: function(callback) {
      new Journal().log("scheduleMicrotask");
      // For V8 engine, you might use queueMicrotask or Promise.resolve().then()
      if (typeof queueMicrotask === 'function') {
        queueMicrotask(callback);
      } else {
        Promise.resolve().then(callback);
      }
    },

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

  const reconciler = ReactReconciler(ReactHostConfig);

  const CustomRenderer = {
    render(element, containerNode, callback) {
      journal.log("CustomRenderer::render");
      const container = reconciler.createContainer(containerNode, false, false);
      reconciler.updateContainer(element, container, null, callback);
    }
  };