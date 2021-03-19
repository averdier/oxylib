import OxylibConfig from '@/oxylib.config.json'

const isValidDeviceId = (deviceId) => {
  return deviceId
    && deviceId.length === 8
    && Object.keys(OxylibConfig.devices).includes(deviceId.substr(0, 2))
    && Object.keys(OxylibConfig.tanks).includes(deviceId.substr(2, 2))
}

const defaultState = () => ({
  status: '',
  message: null,
  error: null,
  device: null,
  tank: null,
  connected: false,
  characteristics: {
    temperature: null,
    pressure: null,
    humidity: null,
    oxygen: null
  }
})

export const namespaced = true
export const state = defaultState

export const mutations = {
  SET_STATUS(state, status) {
    state.status = status
  },
  SET_ERROR(state, error) {
    state.error = error
  },
  SET_MESSAGE(state, message) {
    state.message = message
  },
  SET_DEVICE(state, device) {
    state.device = device
  },
  SET_TANK(state, tank) {
    state.tank = tank
  },
  SET_CONNECTED(state, status) {
    state.connected = status
  },
  SET_CHARACTERISTIC(state, {
    key,
    characteristic
  }) {
    state.characteristics[key] = characteristic
  }
}

export const actions = {
  async connect({
    commit,
    state
  }, deviceId) {
    try {
      if (!isValidDeviceId(deviceId)) throw "Unknown device ID"

      const deviceType = deviceId.substr(0, 2)
      const tankType = deviceId.substr(2, 2)

      commit('SET_MESSAGE', 'Please select your device')
      commit('SET_STATUS', 'loading')

      // Request device if not already exist in memory
      if (!state.device) {
        commit('SET_DEVICE', await navigator.bluetooth.requestDevice({
          filters: [{
            name: `oxylib-${deviceId}`
          }],
          optionalServices: [
            OxylibConfig.devices[deviceType].service
          ]
        }))
      }

      // Subscribe to disconnect event
      state.device.addEventListener('gattserverdisconnected', () => {
        commit('SET_CONNECTED', false)
      }, false)

      // Connect to Gatt server
      commit('SET_MESSAGE', 'Connecting to your device')
      const server = await state.device.gatt.connect()
      commit('SET_CONNECTED', true)

      // Get oxylib ble service
      commit('SET_MESSAGE', 'Getting Oxylib service')
      const service = await server.getPrimaryService(
        OxylibConfig.devices[deviceType].service
      )

      // Get service characteristics
      commit('SET_MESSAGE', "It's almost done..")

      const characs = Object.keys(OxylibConfig.devices[deviceType].characteristics)

      // Add to store first
      for (let i = 0; i < characs.length; i++) {
        const characteristic = {
          tx: await service.getCharacteristic(
            OxylibConfig.devices[deviceType].characteristics[characs[i]].tx
          )
        }
        if (OxylibConfig.devices[deviceType].characteristics[characs[i]].rx) {
          characteristic.rx = await service.getCharacteristic(
            OxylibConfig.devices[deviceType].characteristics[characs[i]].rx
          )
        }
        commit('SET_CHARACTERISTIC', {
          key: characs[i],
          characteristic
        })
      }
      
      // Start notifications
      for (let i = 0; i < characs.length; i++) {
        console.log('getting notifications')
        await state.characteristics[characs[i]].tx.startNotifications()
        console.log('done')
      }

      commit('SET_TANK', OxylibConfig.tanks[tankType])
      commit('SET_STATUS', 'success')

    } catch (error) {
      commit('SET_STATUS', 'error')
      commit('SET_ERROR', error)
      throw error
    }
  },

  async disconnect({
    commit,
    state
  }) {
    // Clean characteristics
    if (state.characteristics.temperature) {
      await state.characteristics.temperature.tx.stopNotifications()
      commit('SET_CHARACTERISTIC', {
        key: 'temperature',
        characteristic: null
      })
    }

    if (state.characteristics.humidity) {
      await state.characteristics.humidity.tx.stopNotifications()
      commit('SET_CHARACTERISTIC', {
        key: 'humidity',
        characteristic: null
      })
    }

    if (state.characteristics.pressure) {
      await state.characteristics.pressure.tx.stopNotifications()
      commit('SET_CHARACTERISTIC', {
        key: 'pressure',
        characteristic: null
      })
    }

    if (state.characteristics.oxygen) {
      await state.characteristics.oxygen.tx.stopNotifications()
      commit('SET_CHARACTERISTIC', {
        key: 'oxygen',
        characteristic: null
      })
    }

    // Disconnect device
    if (state.device) {
      state.device.gatt.disconnect()
    }

    commit('SET_STATUS', 'disconnected')
  }
}