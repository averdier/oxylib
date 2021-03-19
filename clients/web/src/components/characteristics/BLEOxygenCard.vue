<template>
  <v-card>
    <v-card-title>Oxygen</v-card-title>

    <v-card-text class="text-center">
      <p :class="`display-3 ${textColor}`">{{ oxygen }}%</p>
    </v-card-text>

    <v-card-actions class="justify-center">
      <v-btn icon
        :disabled="!canDecrease"
        @click="onDecreaseClick">
        <v-icon>mdi-minus</v-icon>
      </v-btn>

      <v-btn icon
        :disabled="!canIncrease"
        @click="onIncreaseClick">
        <v-icon>mdi-plus-circle-outline</v-icon>
      </v-btn>
    </v-card-actions>
  </v-card>
</template>

<script>
import { mapState } from 'vuex'
import CharacteristicFollowerMixin from '@/mixins/characteristicFollower'

export default {
  name: 'BLEOxygenCard',
  mixins: [
    CharacteristicFollowerMixin
  ],
  data: () => ({
    oxygen: -1,
    step: 10
  }),
  computed: 
  {
    ...mapState({
      connected: state => state.ble.connected,
      characteristic: state => state.ble.characteristics.oxygen
    }),

    canIncrease () {
      return this.oxygen + this.step <= 100
    },
    canDecrease () {
      return this.oxygen - this.step >= 0
    },

    textColor () {
      if (this.oxygen >= 0 && this.oxygen < 25) return 'red--text'
      if (this.oxygen >= 25 && this.oxygen < 50) return 'deep-orange--text'
      if (this.oxygen >= 50 && this.oxygen < 75) return 'orange--text'
      if (this.oxygen >= 75) return 'green--text'

      return ''
    }

  },
  methods: {
    onEvent (event) {
      console.log(event)
      this.oxygen = event.target.value.getUint8(0)
    },

    setOxygen (value) {
      if (this.characteristic.rx) {
        this.characteristic.rx.writeValue(new Uint8Array([value]))
      }
    },

    onIncreaseClick () {
      if (this.oxygen === -1) this.oxygen = 0
      this.setOxygen(this.oxygen + this.step)
    },

    onDecreaseClick () {
      this.setOxygen(this.oxygen - this.step)
    }
  }
}
</script>
