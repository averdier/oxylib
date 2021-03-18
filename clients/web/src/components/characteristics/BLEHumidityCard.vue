<template>
  <v-card>
    <v-card-title>Humidity</v-card-title>
    <v-card-text>
      <p class="display-3">{{ humidity }}%</p>
    </v-card-text>
  </v-card>
</template>

<script>
import { mapState } from 'vuex'
import CharacteristicFollowerMixin from '@/mixins/characteristicFollower'

export default {
  name: 'BLEHumidityCard',
  mixins: [
    CharacteristicFollowerMixin
  ],
  data: () => ({
    humidity: -1
  }),
  computed: mapState({
    connected: state => state.ble.connected,
    characteristic: state => state.ble.characteristics.humidity
  }),
  methods: {
    onEvent (event) {
      const raw = new TextDecoder().decode(event.target.value.buffer)
      this.humidity = raw.substr(0, 2)
    }
  }
}
</script>
