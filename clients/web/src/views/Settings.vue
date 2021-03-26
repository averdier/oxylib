<template>
  <v-container>
    <v-row>
      <v-col cols="12" lg="8" offset-lg="2">
        
        <v-card shaped>
          <v-card-text>
            <v-list two-line subheader>
              <v-subheader>Connectivity</v-subheader>
              
              <!--Wifi-->
              <v-list-item>
                <v-list-item-content>
                  <v-list-item-title>WiFi</v-list-item-title>
                  <v-list-item-subtitle>WiFi is used for cloud functionalities</v-list-item-subtitle>
                </v-list-item-content>
                <v-list-item-action>
                  <v-switch
                    :value="wifiEnabled"
                    :loading="loadingSettings"
                    @change="onWifiSwitchChange"
                    inset
                  ></v-switch>
                </v-list-item-action>
              </v-list-item>

            </v-list>
          </v-card-text>
        </v-card>

      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import { mapState } from 'vuex'

export default {
  name: 'Settings',
  data: () => ({
    loadingSettings: true,
    wifiEnabled: false,
  }),
  computed: {
    ...mapState({
      wifiEnabledCharacteristic: state => state.ble.characteristics.wifiEnabled
    })
  },
  methods: {
    async loadSettings () {
      this.loadingSettings = true
      if (this.wifiEnabledCharacteristic) {
        let bleData = await this.wifiEnabledCharacteristic.tx.readValue()
        this.wifiEnabled = bleData.getUint8(0) ? true : false
      }
      this.loadingSettings = false
    },

    onWifiSwitchChange (enabled) {
      if (this.wifiEnabledCharacteristic) {
        this.loadingSettings = true
        this.wifiEnabledCharacteristic.rx.writeValue(new Uint8Array([enabled ? 1 : 0]))
          .then(() => {
            this.loadSettings()
          })
      }
    }
  },
  mounted () {
    this.loadSettings()
  }
}
</script>