import Vue from 'vue'
import App from './App.vue'
import './registerServiceWorker'
import router from './router'
import store from './store'
import vuetify from './plugins/vuetify';

Vue.config.productionTip = false

if ('serviceWorker' in navigator) {
  let refreshing

  navigator.serviceWorker.addEventListener('controllerchange', function () {
    if (refreshing) return
    else {
      refreshing = true
      window.location.reload()
    }
  })
}

new Vue({
  router,
  store,
  vuetify,
  render: h => h(App)
}).$mount('#app')
