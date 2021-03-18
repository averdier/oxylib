<template>
  <v-card>
    <v-card-title>Temperature</v-card-title>
    <v-card-text>
       <p class="display-3">{{ temperature }}°C</p>
    </v-card-text>
  </v-card>
</template>

<script>
import { mapState } from 'vuex'
import CharacteristicFollowerMixin from '@/mixins/characteristicFollower'

export default {
  name: 'BLETemperatureCard',
  mixins: [
    CharacteristicFollowerMixin
  ],
  data: () => ({
    temperature: -1
  }),
  computed: mapState({
    connected: state => state.ble.connected,
    characteristic: state => state.ble.characteristics.temperature
  }),
  methods: {
    onEvent (event) {
      const raw = new TextDecoder().decode(event.target.value.buffer)
      this.temperature = raw.substr(0, 2)
    }
  }
}
</script>
