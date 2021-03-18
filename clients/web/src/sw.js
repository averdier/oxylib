  
workbox.core.setCacheNameDetails({
  prefix: "oxylib"
})

self.__precacheManifest = [].concat(self.__precacheManifest || [])
workbox.precaching.precacheAndRoute(self.__precacheManifest, {})

workbox.routing.registerNavigationRoute('/index.html')

self.addEventListener("message", event => {
  if (event.data === "skipWaiting") {
      self.skipWaiting()
  }
})