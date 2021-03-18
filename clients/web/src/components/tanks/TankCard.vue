<template>
  <v-card>
    <v-img :src="item.picture" height="200px" />
    <v-card-title>Tank</v-card-title>
    <v-card-subtitle>{{ item.name }}</v-card-subtitle>
  </v-card>
</template>

<script>
export default {
  name: 'TankCard',
  props: {
    item: {
      type: Object,
      required: true
    }
  }
}
</script>