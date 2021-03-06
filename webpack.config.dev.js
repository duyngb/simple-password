//@ts-check
let path = require('path');
let baseConfig = require('./webpack.config.base');

baseConfig.devServer = {
  compress: false,
  port: 9000,
  hot: true,
  open: false,
  headers: {
    'Connection': 'close',
    'Cache-Control': 'no-store, no-cache',
  },
  static: path.join(__dirname, "docs"),
};
baseConfig.mode = 'development';
baseConfig.devtool = 'inline-source-map';
baseConfig.watchOptions = {
  ignored: ['**/node_modules/**', '**/*.re'],
};

module.exports = baseConfig;
