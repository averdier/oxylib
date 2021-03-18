import Vue from 'vue'
import Vuex from 'vuex'
import * as settings from './modules/settings'
import * as ble from './modules/ble'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
  },
  mutations: {
  },
  actions: {
  },
  modules: {
    settings,
    ble
  }
})
