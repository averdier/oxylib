export default {
  methods: {
    followEvents () {
      if (this.characteristic) {
        this.characteristic.addEventListener('characteristicvaluechanged', this.onEvent)
      }
    },

    unFollowEvents () {
      if (this.characteristic) {
        this.characteristic.removeEventListener('characteristicvaluechanged', this.onEvent)
      }
    }
  },

  mounted () {
    this.followEvents()
  },
  
  beforeDestroy () {
    this.unFollowEvents()
  }
}