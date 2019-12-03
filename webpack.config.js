//@ts-check
let baseConfig = require('./webpack.config.base');

baseConfig.mode = 'production';
baseConfig.devtool = undefined;

module.exports = baseConfig;
