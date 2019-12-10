//@ts-check
let path = require('path');

/** @type {import('webpack').Configuration} */
module.exports = {
  target: "web",
  entry: {
    index: './src/Index.bs.js',
  },
  output: {
    path: path.join(__dirname, 'bundleOutput'),
    filename: '[name].js',
  },
  optimization: {
    moduleIds: 'deterministic',
    runtimeChunk: 'single',
    splitChunks: {
      chunks: 'all',
      cacheGroups: {
        vendor: {
          test: /[\\/]node_modules[\\/]/,
          name: 'vendors',
          chunks: 'all'
        }
      }
    }
  }
};
