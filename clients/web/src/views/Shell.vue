<template>
  <div class="fill-height">
    <v-app-bar app color="white" flat>

      <!--Mobile app bar-->
      <template v-if="$vuetify.breakpoint.smAndDown">
        <v-app-bar-nav-icon @click.stop="drawer = !drawer"></v-app-bar-nav-icon>
        <v-toolbar-title>{{ $route.meta && $route.meta.label || 'Oxylib' }}</v-toolbar-title>
      </template>

      <!--Tablet and pc app bar-->
      <template v-else>
        <v-tabs centered class="ml-n9" color="grey darken-1">
          <v-tab
            v-for="(link, index) in links"
            :key="`${index}-tab`"
            :to="link.to">{{ link.label }}</v-tab>
        </v-tabs>

        <v-btn icon @click="onDisconnectClick">
          <v-icon>mdi-bluetooth-off</v-icon>
        </v-btn>
      </template>
    </v-app-bar>

    <!--Mobile drawer-->
    <v-navigation-drawer v-if="$vuetify.breakpoint.smAndDown" v-model="drawer" app>
      <v-list dense nav>

        <v-list-item link
          v-for="(link, index) in links"
          :key="index"
          :to="link.to">
          
          <v-list-item-icon>
            <v-icon>{{ link.icon }}</v-icon>
          </v-list-item-icon>

          <v-list-item-content>
            <v-list-item-title>{{ link.label }}</v-list-item-title>
          </v-list-item-content>

        </v-list-item>
      </v-list>

      <template v-slot:append>
        <div class="pa-2">
          <v-list-item @click="onDisconnectClick">
            
            <v-list-item-icon>
              <v-icon>mdi-bluetooth-off</v-icon>
            </v-list-item-icon>

            <v-list-item-content>
              <v-list-item-title>Disconnect</v-list-item-title>
            </v-list-item-content>

          </v-list-item>
        </div>
      </template>
    </v-navigation-drawer>

    <v-main class="fill-height grey lighten-3">
      <router-view></router-view>
    </v-main>
  </div>
</template>

<script>
import { mapState } from 'vuex'

export default {
  name: 'Shell',
  data: () => ({
    drawer: null,
    links: [
      { label: 'Home', icon: 'mdi-view-dashboard', to: { name: 'Home' } },
      { label: 'Settings', icon: 'mdi-cog', to: { name: 'Settings' }},
      { label: 'About', icon: 'mdi-information-outline', to: { name: 'NestedAbout' } }
    ]
  }),
  computed: mapState({
    connected: state => state.ble.connected
  }),
  methods: {
    onDisconnectClick () {
      this.$store.dispatch('ble/disconnect')
    }
  },
  watch: {
    connected: function (val) {
      if (!val) {
        this.$router.push({ name: 'Start' })
      }
    }
  },
  mounted () {
    // Shell is a proxy, so redirect to home view
    if (this.$route.name === 'Shell') {
      this.$router.push({ name: 'Home' })
    }
  }
}
</script>