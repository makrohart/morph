import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// 根据命令行参数决定配置
const mode = process.argv.includes('--mode') ? process.argv[process.argv.indexOf('--mode') + 1] : 'development';
const isDebug = process.argv.includes('--debug') || process.env.NODE_ENV === 'debug' || process.env.npm_lifecycle_event === 'build:debug';
const isDev = mode === 'development' && !isDebug;
const isProd = mode === 'production';

export default {
    entry: isDebug ? ['./mocks/index.js', './index.js'] : './index.js',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: isDebug ? 'bundle.debug.js' : (isDev ? 'bundle.dev.js' : 'bundle.prod.js'),
        library: 'MorphlingApp',
        libraryTarget: 'umd',
        globalObject: 'globalThis'
    },
    module: {
        rules: [
            {
                test: /\.js$/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                    options: {
                        presets: [
                            ['@babel/preset-env', {
                                targets: {
                                    node: '14'
                                }
                            }],
                            ['@babel/preset-react', {
                                runtime: 'automatic'
                            }]
                        ]
                    }
                }
            }
        ]
    },
    resolve: {
        extensions: ['.js', '.jsx'],
        alias: {
            'react': path.resolve(__dirname, 'node_modules/react'),
            'react-reconciler': path.resolve(__dirname, 'node_modules/react-reconciler'),
            // 在debug模式下，使用mock文件替代external
            ...(isDebug && {
                'View': path.resolve(__dirname, 'mocks/View.js'),
                'DivView': path.resolve(__dirname, 'mocks/DivView.js'),
                'ButtonView': path.resolve(__dirname, 'mocks/ButtonView.js'),
                'WindowView': path.resolve(__dirname, 'mocks/WindowView.js'),
                'TextView': path.resolve(__dirname, 'mocks/TextView.js'),
                'ImageView': path.resolve(__dirname, 'mocks/ImageView.js'),
                'ScrollView': path.resolve(__dirname, 'mocks/ScrollView.js'),
                'ListView': path.resolve(__dirname, 'mocks/ListView.js'),
                'Journal': path.resolve(__dirname, 'mocks/Journal.js'),
                'MorphTimer': path.resolve(__dirname, 'mocks/MorphTimer.js')
            })
        }
    },
    externals: isDebug ? {} : {
        // 非debug模式下，这些是V8环境提供的全局对象，不需要打包
        'View': 'View',
        'DivView': 'DivView',
        'ButtonView': 'ButtonView',
        'WindowView': 'WindowView',
        'TextView': 'TextView',
        'ImageView': 'ImageView',
        'ScrollView': 'ScrollView',
        'ListView': 'ListView',
        'Journal': 'Journal',
        'MorphTimer': 'MorphTimer'
    },
    mode: mode,
    devtool: 'source-map',  // 所有模式都生成source map
    target: 'node',
    optimization: {
        minimize: isProd,  // 只有生产模式压缩代码
        usedExports: false,  // 不标记未使用的导出
        sideEffects: false
    }
};
