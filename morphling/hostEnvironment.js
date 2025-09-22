// 定义全局变量
if (typeof globalThis !== 'undefined') {
    // 现代环境 - 保存原始console
    globalThis.originalConsole = globalThis.console;
    globalThis.console = new Journal();
    globalThis.morphTimer = new MorphTimer();
} else if (typeof global !== 'undefined') {
    // Node.js 环境 - 保存原始console
    global.originalConsole = global.console;
    global.console = new Journal();
    global.morphTimer = new MorphTimer();
} else if (typeof window !== 'undefined') {
    // 浏览器环境 - 保存原始console
    window.originalConsole = window.console;
    window.console = new Journal();
    window.morphTimer = new MorphTimer();
} else {
    // V8 环境 - 直接赋值给 this
    this.originalConsole = this.console;
    this.console = new Journal();
    this.morphTimer = new MorphTimer();
}

// 创建局部引用
const console = globalThis?.console || global?.console || window?.console || this.console;
const morphTimer = globalThis?.morphTimer || global?.morphTimer || window?.morphTimer || this.morphTimer;

// 正确的全局定义方式
if (typeof globalThis !== 'undefined') {
    // 现代环境 - 保存原始函数
    globalThis.originalSetTimeout = globalThis.setTimeout;
    globalThis.originalClearTimeout = globalThis.clearTimeout;
    globalThis.setTimeout = (callback, delayMS) => {
        return morphTimer.setTimeout(callback, delayMS);
    };
    globalThis.clearTimeout = (id) => {
        morphTimer.clearTimeout(id);
    };
    globalThis.setImmediate = (fn, ...args) => {
        return globalThis.setTimeout(fn, 0, ...args);
    };
} else if (typeof global !== 'undefined') {
    // Node.js 环境 - 保存原始函数
    global.originalSetTimeout = global.setTimeout;
    global.originalClearTimeout = global.clearTimeout;
    global.setTimeout = (callback, delayMS) => {
        return morphTimer.setTimeout(callback, delayMS);
    };
    global.clearTimeout = (id) => {
        morphTimer.clearTimeout(id);
    };
    global.setImmediate = (fn, ...args) => {
        return global.setTimeout(fn, 0, ...args);
    };
} else if (typeof window !== 'undefined') {
    // 浏览器环境 - 保存原始函数
    window.originalSetTimeout = window.setTimeout;
    window.originalClearTimeout = window.clearTimeout;
    window.setTimeout = (callback, delayMS) => {
        return morphTimer.setTimeout(callback, delayMS);
    };
    window.clearTimeout = (id) => {
        morphTimer.clearTimeout(id);
    };
    window.setImmediate = (fn, ...args) => {
        return window.setTimeout(fn, 0, ...args);
    };
} else {
    // V8 环境 - 直接赋值给 this
    this.originalSetTimeout = this.setTimeout;
    this.originalClearTimeout = this.clearTimeout;
    this.setTimeout = (callback, delayMS) => {
        return morphTimer.setTimeout(callback, delayMS);
    };
    this.clearTimeout = (id) => {
        morphTimer.clearTimeout(id);
    };
    this.setImmediate = (fn, ...args) => {
        return this.setTimeout(fn, 0, ...args);
    };
}

const HostEnvironment = {
    isValidNode: (node) => {
        return node instanceof View ||
               node instanceof DivView ||
               node instanceof ButtonView ||
               node instanceof WindowView ||
               node instanceof TextView ||
               node instanceof ImageView ||
               node instanceof ScrollView ||
               node instanceof ListView;
    },

    createNode: (type) => {
        new Journal().log("createNode");
        switch (type) {
            case "button":
                return new ButtonView();
            case "div":
                return new DivView();
            case "window":
                return new WindowView();
            case "image":
                return new ImageView();
            case "scroll":
                return new ScrollView();
            case "list":
                return new ListView();
            case "text":
                return new TextView();
            default:
                console.warn(`Unknown node type: ${type}, falling back to DivView`);
                return new DivView();
        }
    },

    createTextNode: (textInstance) => {
      const textView = new TextView(textInstance.text);
      return textView;
    },

    addNode: (parent, child) => {
        new Journal().log("addNode");
        if (HostEnvironment.isValidNode(child) && HostEnvironment.isValidNode(parent)) {
            child.addTo(parent);
        } else {
            console.log("Invalid node or parent in addNode operation");
        }
    },

    removeNode: (parent, child) => {
        new Journal().log("removeNode");
        if (HostEnvironment.isValidNode(child) && HostEnvironment.isValidNode(parent)) {
            child.removeFrom(parent);
        } else {
            console.log("Invalid node or parent in removeNode operation");
        }
    },

    // 添加属性设置方法
    setProperty: (node, property, value) => {
        if (HostEnvironment.isValidNode(node) && node.setProperty) {
            node.setProperty(property, value);
        } else {
            console.log(`Cannot set property ${property} on node:`, node);
        }
    },

    // 添加事件绑定方法
    addEventListener: (node, eventType, handler) => {
        if (HostEnvironment.isValidNode(node) && node.onEvent) {
            node.onEvent(eventType, handler);
        } else {
            console.log(`Cannot add event listener ${eventType} to node:`, node);
        }
    }
};

export default HostEnvironment;