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
      chunks: 'async',
      minChunks: 1,
      minSize: 1024*30,
      maxAsyncRequests: 6,
      cacheGroups: {
        vendors: {
          chunks: 'all',
          test: /[\\/]node_modules[\\/]/,
          priority: -20,
          name: 'vendors',
          reuseExistingChunk: true,
          enforce: true
        }
      }
    }
  }
};
