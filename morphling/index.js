import React from 'react';
import CustomRenderer from './morphling.js';
import { App } from './app.js';

// 创建根容器
const index_html = new View();

// 渲染React应用
CustomRenderer.render(React.createElement(App), index_html);