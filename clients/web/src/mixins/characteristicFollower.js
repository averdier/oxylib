export default {
  methods: {
    followEvents () {
      if (this.characteristic) {
        this.characteristic.tx.addEventListener('characteristicvaluechanged', this.onEvent)
        this.characteristic.tx.readValue()
          .then(data => {
            if (this.onData) this.onData(data)
          })
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