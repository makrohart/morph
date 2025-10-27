// Mock入口文件 - 在开发环境中提供V8 API的模拟实现
import { View } from './View.js';
import { DivView } from './DivView.js';
import { ButtonView } from './ButtonView.js';
import { WindowView } from './WindowView.js';
import { TextView } from './TextView.js';
import { ImageView } from './ImageView.js';
import { ScrollView } from './ScrollView.js';
import { ListView } from './ListView.js';
import { Journal } from './Journal.js';
import { MorphTimer } from './MorphTimer.js';

// 导出这些类
export { View, DivView, ButtonView, WindowView, TextView, ImageView, ScrollView, ListView, Journal, MorphTimer };

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
