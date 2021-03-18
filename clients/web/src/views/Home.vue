<template>
  <v-container>
    <v-row>
      <v-col cols="12">
        <TankCard v-if="tank" :item="tank" />
      </v-col>
      <v-col cols="12">
        <BLEOxygenCard />
      </v-col>
      <v-col cols="12" sm="6" md="4"
        v-for="(component, i) in characteristicCards"
        :key="i">
        <component :is="component" />
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import { mapState } from 'vuex'

import BLEHumidityCard from '@/components/characteristics/BLEHumidityCard.vue'
import BLEPressureCard from '@/components/characteristics/BLEPressureCard.vue'
import BLETemperatureCard from '@/components/characteristics/BLETemperatureCard.vue'
import BLEOxygenCard from '@/components/characteristics/BLEOxygenCard.vue'
import TankCard from '@/components/tanks/TankCard.vue'

export default {
  name: 'Home',
  components: {
    BLEHumidityCard,
    BLEPressureCard,
    BLETemperatureCard,
    BLEOxygenCard,
    TankCard
  },
  computed: {
    ...mapState({
      tank: state => state.ble.tank, 
      characteristics: state => state.ble.characteristics
    }),

    characteristicCards () {
      const result = []

      if (this.characteristics.temperature) result.push(BLETemperatureCard)
      if (this.characteristics.humidity) result.push(BLEHumidityCard)
      if (this.characteristics.pressure) result.push(BLEPressureCard)

      return result
    }
  }
}
</script>