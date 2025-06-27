import resolve from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
import replace from '@rollup/plugin-replace';
import { terser } from 'rollup-plugin-terser';
import path from 'path';

export default {
  input: path.resolve(__dirname, 'src/index.js'),
  output: {
    file: path.resolve(__dirname, 'dist/reacts.umd.js'),
    format: 'umd',
    name: 'Reacts',
    globals: {}, // 必须为空，确保不依赖外部全局变量
  },
  plugins: [
    // 替换 Node.js 环境变量为浏览器兼容值
    replace({
      'process.env.NODE_ENV': JSON.stringify('production'),
      'process.browser': true,
      'typeof module': JSON.stringify('undefined'),
      'process.nextTick': 'setTimeout',
      preventAssignment: true, // 避免替换导致赋值错误
    }),
    // 解析第三方依赖
    resolve({
      browser: true, // 强制使用浏览器版本
    }),
    // 转换 CommonJS 为 ES 模块
    commonjs({
      transformMixedEsModules: true,
      include: /node_modules/, // 明确处理 node_modules
    }),
    // 压缩代码（可选）
    // terser(),
  ],
  external: [], // 必须为空，打包所有依赖
};