import Vue from 'vue'
import VueRouter from 'vue-router'
import Store from '../store'

import Shell from '../views/Shell.vue'
import Start from '../views/Start.vue'
import Requirements from '../views/Requirements.vue'
import Home from '../views/Home.vue'
import About from '../views/About.vue'

Vue.use(VueRouter)

const needBluetoothGuard = (to, from, next) => {
  if ('bluetooth' in navigator) next()
  else next({ name: 'Requirements' })
}

const connectedGuard = (to, from, next) => {
  if (Store.state.ble.connected) next()
  else next({ name: 'Start' })
}

const routes = [
  {
    path: '/',
    name: 'Shell',
    component: Shell,
    beforeEnter: connectedGuard,
    children: [
      {
        path: 'home',
        name: 'Home',
        component: Home
      },
      {
        path: 'about',
        name: 'NestedAbout',
        component: About
      }
    ]
  },
  {
    path: '/start',
    name: 'Start',
    component: Start,
    beforeEnter: needBluetoothGuard
  },
  {
    path: '/requirements',
    name: 'Requirements',
    component: Requirements
  },
  {
    path: '/about',
    name: 'About',
    // route level code-splitting
    // this generates a separate chunk (about.[hash].js) for this route
    // which is lazy-loaded when the route is visited.
    component: () => import(/* webpackChunkName: "about" */ '../views/About.vue')
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
