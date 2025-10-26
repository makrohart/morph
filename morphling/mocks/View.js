// Mock for View class
export class View {
    constructor() {
        this.children = [];
        this.parent = null;
        this.properties = {};
        this.eventHandlers = {};
    }

    addTo(parent) {
        if (parent && parent.children) {
            parent.children.push(this);
            this.parent = parent;
            console.log(`View added to parent`);
        }
    }

    removeFrom(parent) {
        if (parent && parent.children) {
            const index = parent.children.indexOf(this);
            if (index > -1) {
                parent.children.splice(index, 1);
                this.parent = null;
                console.log(`View removed from parent`);
            }
        }
    }

    setProperty(property, value) {
        this.properties[property] = value;
        console.log(`Property ${property} set to:`, value);
    }

    onEvent(eventType, handler) {
        this.eventHandlers[eventType] = handler;
        console.log(`Event handler for ${eventType} added`);
    }

    // 添加事件移除方法
    removeEvent(eventType, handler) {
        if (this.eventHandlers[eventType]) {
            delete this.eventHandlers[eventType];
            console.log(`Event handler for ${eventType} removed`);
        } else {
            console.log(`No event handler found for ${eventType}`);
        }
    }

    // 模拟事件触发
    triggerEvent(eventType, ...args) {
        if (this.eventHandlers[eventType]) {
            this.eventHandlers[eventType](...args);
        }
    }
}
