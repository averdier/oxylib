export default {
  methods: {
    followEvents () {
      if (this.characteristic) {
        console.log('subscribe')
        this.characteristic.tx.addEventListener('characteristicvaluechanged', this.onEvent)
      }
    },

    unFollowEvents () {
      if (this.characteristic) {
        this.characteristic.tx.removeEventListener('characteristicvaluechanged', this.onEvent)
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