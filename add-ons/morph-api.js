// Minimal Node.js-like polyfills for v8

// // 1. global
// var global = this;

// // 2. exports and module
// var exports = {};
// var module = { exports: exports };

// // 3. process
// var process = {};
// process.emit = function () {};
// process.env = {};
// process.nextTick = function (cb) { setTimeout(cb, 0); };

// // 4. performance
// var performance = {
//     now: function () { return Date.now(); }
// };

var console = new Journal();

var morphTimer = new MorphTimer();
setTimeout = (callback, delayMS) => {
    return morphTimer.setTimeout(callback, delayMS);
};
setImmediate = (fn, ...args) => {
    return setTimeout(fn, 0, ...args);
};
clearTimeout= (id) => {
    morphTimer.clearTimeout(id);
};

// class MorphNode{
//     constructor()
//     {
//         console.log("new MorphNode");
//     }

//     add(node)
//     {
//         if (node instanceof MorphNode)
//             console.log("node instanceof MorphNode");
//         else
//             console.log("not node instanceof MorphNode");
//     }

//     remove(node)
//     {
//         console.log("MorphNode::remove");
//     }
// }


