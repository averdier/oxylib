// Manage user settings

const defaultState = () => {
  const localSettings = JSON.parse(localStorage.getItem(
    `${process.env.VUE_APP_NAME}-settings`
  ) || "{}")

  return {
    deviceId: localSettings.deviceId || null
  }
}

export const namespaced = true
export const state = defaultState()

export const mutations = {
  SET_SETTING (state, { key, value }) {
    if (state[key] !== undefined) {
      state[key] = value
      localStorage.setItem(
        `${process.env.VUE_APP_NAME}-settings`,
        JSON.stringify(state)
      )
    }
  }
}

export const actions = {
  set({ commit }, { key, value }) {
    commit('SET_SETTING', { key, value })
  }
}