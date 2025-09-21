// Mock入口文件 - 在开发环境中提供V8 API的模拟实现
export { View } from './View.js';
export { DivView } from './DivView.js';
export { ButtonView } from './ButtonView.js';
export { WindowView } from './WindowView.js';
export { TextView } from './TextView.js';
export { ImageView } from './ImageView.js';
export { ScrollView } from './ScrollView.js';
export { ListView } from './ListView.js';
export { Journal } from './Journal.js';
export { MorphTimer } from './MorphTimer.js';

// 将mock对象设置为全局变量，模拟V8环境
if (typeof globalThis !== 'undefined') {
    globalThis.View = View;
    globalThis.DivView = DivView;
    globalThis.ButtonView = ButtonView;
    globalThis.WindowView = WindowView;
    globalThis.TextView = TextView;
    globalThis.ImageView = ImageView;
    globalThis.ScrollView = ScrollView;
    globalThis.ListView = ListView;
    globalThis.Journal = Journal;
    globalThis.MorphTimer = MorphTimer;
} else if (typeof global !== 'undefined') {
    global.View = View;
    global.DivView = DivView;
    global.ButtonView = ButtonView;
    global.WindowView = WindowView;
    global.TextView = TextView;
    global.ImageView = ImageView;
    global.ScrollView = ScrollView;
    global.ListView = ListView;
    global.Journal = Journal;
    global.MorphTimer = MorphTimer;
} else if (typeof window !== 'undefined') {
    window.View = View;
    window.DivView = DivView;
    window.ButtonView = ButtonView;
    window.WindowView = WindowView;
    window.TextView = TextView;
    window.ImageView = ImageView;
    window.ScrollView = ScrollView;
    window.ListView = ListView;
    window.Journal = Journal;
    window.MorphTimer = MorphTimer;
}
