import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export default {
    entry: './index.js',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'bundle.js',
        library: 'MorphlingApp',
        libraryTarget: 'umd',
        globalObject: 'this'
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
            'react-reconciler': path.resolve(__dirname, 'node_modules/react-reconciler')
        }
    },
    externals: {
        // 这些是V8环境提供的全局对象，不需要打包
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
    mode: 'development',
    devtool: 'source-map',
    target: 'node',
    optimization: {
        minimize: false,  // 不压缩代码，保持可读性
        usedExports: false,  // 不标记未使用的导出
        sideEffects: false
    }
};
