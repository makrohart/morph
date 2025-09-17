# Morphling - React with V8 Native Components

这是一个使用 React Reconciler 和 V8 引擎的项目，通过自定义渲染器将 React 组件渲染到原生 C++ 组件。

## 项目结构

```
morphling/
├── index.js              # 应用入口点
├── main.js               # React 组件和状态管理
├── morphling.js          # React Reconciler 自定义渲染器
├── hostEnvironment.js    # 原生组件环境适配器
├── webpack.config.js     # Webpack 打包配置
├── .babelrc             # Babel 配置
├── package.json         # 项目依赖
├── dist/                # 打包输出目录
│   ├── bundle.js        # 主要的打包文件 (879KB)
│   └── bundle.js.map    # Source Map 文件 (1MB)
└── README.md            # 项目文档
```

## 核心功能

### 1. React Reconciler 自定义渲染器
- 实现了完整的 React Reconciler Host Config
- 支持优先级调度和微任务
- 处理组件创建、更新、删除等生命周期

### 2. 原生组件支持
- `View` - 基础视图组件
- `DivView` - 容器组件
- `ButtonView` - 按钮组件
- `WindowView` - 窗口组件
- `TextView` - 文本组件
- `ImageView` - 图片组件
- `ScrollView` - 滚动组件
- `ListView` - 列表组件

### 3. 状态管理
- 使用 React Hooks 进行状态管理
- 支持 `useState` 和 `useEffect`
- 响应式数据更新

### 4. 事件系统
- 支持原生事件绑定
- 自动处理 `onClick`、`onChange` 等事件

## 使用方法

### 开发环境
```bash
# 安装依赖
npm install

# 开发模式（监听文件变化）
npm run dev

# 开发构建（生成可读的 bundle.js）
npm run build

# 生产构建（压缩优化）
npm run build:prod
```

### V8 环境集成

#### 1. 加载 bundle.js
```javascript
// 在 V8 环境中加载打包后的文件
loadScript('path/to/dist/bundle.js');
```

#### 2. 应用自动启动
应用会自动启动，因为 `index.js` 中的代码会立即执行，创建一个 `View` 作为根容器并渲染 React 组件。

#### 3. 全局对象要求
确保 V8 环境中定义了以下全局对象：
- **必需对象**：`View`, `DivView`, `ButtonView`, `WindowView`, `TextView`
- **可选对象**：`ImageView`, `ScrollView`, `ListView`
- **系统对象**：`Journal` (日志系统), `MorphTimer` (定时器系统)

## 示例代码

```javascript
import React from 'react';

function App() {
    const [count, setCount] = React.useState(0);
    
    return React.createElement(
        "window",
        { style: { width: '800', height: '600' } },
        React.createElement(
            "button",
            {
                onClick: () => setCount(count + 1),
                style: { widthPercent: "50", heightPercent: "50" }
            },
            `Click Me (${count})`
        )
    );
}
```

## 打包文件说明

### 生成的文件
- **`dist/bundle.js`** (879KB) - 主要的打包文件
- **`dist/bundle.js.map`** (1MB) - Source Map 文件

### bundle.js.map 的作用
Source Map 文件用于：
- **调试支持**：将打包后的代码映射回原始源代码
- **错误定位**：准确定位错误到原始文件的具体行号
- **开发体验**：在开发者工具中直接查看和调试原始代码
- **性能分析**：帮助分析代码执行路径

### 代码结构
生成的 bundle.js 包含：
1. **Webpack 运行时** - 模块加载和管理
2. **React 库** - React 核心功能
3. **React Reconciler** - 自定义渲染器
4. **HostEnvironment** - 原生组件适配器
5. **Morphling 渲染器** - 自定义 React Reconciler 配置
6. **App 组件** - 主要的 React 组件
7. **入口代码** - 自动启动应用的代码

## 技术特点

- **自定义渲染器**: 完全控制组件渲染过程
- **原生性能**: 直接调用 C++ 原生组件
- **React 生态**: 支持 React Hooks 和组件模式
- **状态管理**: 使用 React Hooks 进行状态管理
- **事件处理**: 完整的原生事件支持
- **可读代码**: 生成未压缩的可读 bundle.js
- **调试友好**: 支持 Source Map 和详细日志

## 调试和故障排除

如果遇到问题，可以：
1. **检查全局对象**：确保 V8 环境中正确定义了所有必需的全局对象
2. **查看日志**：通过 Journal 日志输出来跟踪执行流程
3. **使用 Source Map**：在开发者工具中查看 Source Map 来定位问题
4. **检查控制台**：查看是否有 JavaScript 错误或警告

## 注意事项

1. 确保 V8 环境中已正确实现所有原生组件类
2. 样式属性需要根据原生组件 API 进行调整
3. 事件名称需要与原生组件支持的事件类型匹配
4. 建议在生产环境中关闭调试日志以提高性能
5. Source Map 文件仅用于调试，生产环境可以删除
