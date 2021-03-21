<template>
  <v-container class="fill-height grey lighten-3" fluid>
    <v-row justify="center" align="center">
      <v-col cols="12" md="6" lg="4" class="text-center">

        <!--Loading-->
        <template v-if="status === 'loading'">
          <v-progress-circular
            :size="64"
            :width="6"
            color="primary"
            indeterminate
          ></v-progress-circular>
          <p class="body-1 mt-5">{{ message }}</p>
        </template>

        <!--Error-->
        <template v-else-if="status === 'error'">
          <v-icon size="64">
            mdi-alert-circle-outline
          </v-icon>

          <h1 class="mt-5">Something goes wrong...</h1>

          <p>
            {{ error }}
          </p>

          <v-btn @click="connect" class="mt-5">Try again.</v-btn>
        </template>

        <!--Form-->
        <template v-else>
          <div class="pb-10">
            <v-img :src="require('@/assets/logo.png')" contain height="128"></v-img>
          </div>

          <!--Login card-->
          <v-card class="loginCard">
            <v-card-title>Connect</v-card-title>
            <v-card-subtitle>Please enter your device ID</v-card-subtitle>
            <v-card-text>
              <v-form ref="loginForm"
                v-model="loginForm.valid"
                lazy-validation>

                <v-text-field
                  label="Device ID"
                  v-model="loginForm.data.deviceId"
                  :rules="loginForm.rules.deviceId"
                  required />

              </v-form>
            </v-card-text>

            <v-card-actions>
              <v-spacer />
              <v-btn text color="primary"
              :disabled="!loginForm.valid"
              @click="onConnectClick">Connect</v-btn>
            </v-card-actions>
          </v-card>

          <!--Link to about page-->
          <v-btn outlined rounded text class="mt-5"
            :to="{ name: 'About' }">
            About
          </v-btn>
        </template>

      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import { mapState } from 'vuex'
import OxylibConfig from '@/oxylib.config.json'

const deviceIDs = Object.keys(OxylibConfig.devices)
const tankIDs = Object.keys(OxylibConfig.tanks)


export default {
  name: 'Start',
  data: () => ({
    loginForm: {
      valid: false,
      rules: {
        deviceId: [
          v => !!v || 'Device ID is required',
          v => (v && v.length === 8) || 'Device ID must be 8 characters',
          v => (
            v
            && deviceIDs.includes(v.substr(0, 2))
            && tankIDs.includes(v.substr(2, 2))
          ) || 'Unknown device ID'
        ]
      },
      data: {
        deviceId: null
      }
    }
  }),
  computed: {
    ...mapState({
      status: state => state.ble.status,
      message: state => state.ble.message,
      error: state => state.ble.error,
      settings: state => state.settings
    })
  },
  methods: {
    onConnectClick () {
      // If valid form
      if (this.$refs.loginForm.validate()) {
        this.connect()
      }
    },
    connect() {
      // Try connect to device
      this.$store.dispatch('ble/connect', this.loginForm.data.deviceId)
        .then(() => {
          // Save device id in settings
          this.$store.dispatch('settings/set', {
            key: 'deviceId',
            value: this.loginForm.data.deviceId
          })
          
          // Redirect to home
          this.$router.push({ name: 'Home'})
        })
    }
  },
  mounted () {
    // Preload form with local settings
    this.loginForm.data.deviceId = this.settings.deviceId
  }
}
</script>

<style>
.loginCard {
  text-align: initial;
}
</style>