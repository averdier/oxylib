<template>
  <v-card>
    <v-card-title>Pressure</v-card-title>
    <v-card-text>
       <p class="display-3">{{ pressure }} hPa</p>
    </v-card-text>
  </v-card>
</template>

<script>
import { mapState } from 'vuex'
import CharacteristicFollowerMixin from '@/mixins/characteristicFollower'

export default {
  name: 'BLEPressureCard',
  mixins: [
    CharacteristicFollowerMixin
  ],
  data: () => ({
    pressure: -1
  }),
  computed: mapState({
    connected: state => state.ble.connected,
    characteristic: state => state.ble.characteristics.pressure
  }),
  methods: {
    onEvent (event) {
      const raw = new TextDecoder().decode(event.target.value.buffer)
      this.pressure = raw.substr(0, 3)
    }
  }
}
</script>
