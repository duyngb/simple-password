//@ts-check
let path = require('path');
let baseConfig = require('./webpack.config.base');

baseConfig.devServer = {
  contentBase: path.join(__dirname),
  compress: false,
  port: 9000,
  hot: true,
  open: false,
  headers: {
    'Connection': 'close',
    'Cache-Control': 'no-store, no-cache',
  }
};
baseConfig.mode = 'development';
baseConfig.devtool = 'inline-source-map';

module.exports = baseConfig;
