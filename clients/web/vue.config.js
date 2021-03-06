module.exports = {
  transpileDependencies: [
    'vuetify'
  ],
  pwa: {
    workboxPluginMode: 'InjectManifest',
    workboxOptions: {
      swSrc: 'src/sw.js',
      swDest: 'service-worker.js',
      importWorkboxFrom: 'local',
      exclude: [/\.map$/, /_redirects/]
    }
  }
}
