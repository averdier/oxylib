<template>
  <v-container class="fill-height grey lighten-3" fluid>
    <v-row justify="center" align="center">
      <v-col cols="12" md="6" lg="4" class="text-center">

        <v-card class="requirementsCard">
          <v-card-title>Sorry...</v-card-title>
          <v-card-subtitle>You does not meet the requirements</v-card-subtitle>
          <v-card-text>
            <v-list rounded>

              <v-list-item>
                <v-list-item-icon>
                  <v-icon>mdi-bluetooth</v-icon>
                </v-list-item-icon>
                <v-list-item-content>
                  <v-list-item-title>Bluetooth</v-list-item-title>
                </v-list-item-content>
                <v-list-item-action>
                  <v-icon :color="bluetooth ? 'green' : 'red'">{{ bluetooth ? 'mdi-check' : 'mdi-close' }}</v-icon>
                </v-list-item-action>
              </v-list-item>

            </v-list>

            <p>
              This can be your device or your brower. <br/>
              <a href="https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API">See compatibility</a>
            </p>
          </v-card-text>
        </v-card>

      </v-col>
    </v-row>
  </v-container>
</template>

<script>
export default {
  name: 'Requirements',
  data: () => ({
    bluetooth: 'bluetooth' in navigator
  })
}
</script>

<style>
.requirementsCard {
  text-align: initial;
}
</style>