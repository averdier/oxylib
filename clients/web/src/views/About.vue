<template>
  <v-container class="grey lighten-3" :fluid="fluid">
    <v-row>
      <v-col cols="12">
        <h1>About project</h1>
      </v-col>
      <v-col cols="12" lg="3">
        <v-img src="https://via.placeholder.com/2000" height="300px" />
      </v-col>
      <v-col cols="12" lg="9">
        Lorem ipsum dolor sit amet, consectetur adipiscing elit.
        Cras sed tempor tellus. Nulla pretium elementum aliquam.
        Phasellus sodales odio id tellus dictum venenatis.
        Pellentesque viverra, dolor ut lacinia interdum, turpis sem rhoncus libero, eu porta felis leo vitae elit.
        Quisque finibus commodo lorem, ut finibus erat.
        Proin nisl tellus, bibendum efficitur dui at, molestie volutpat urna.
        Sed accumsan tortor et varius convallis. Proin eget quam dolor.
        Donec efficitur, nunc id feugiat ornare, massa odio luctus orci, et auctor leo nunc in ex.
        Duis urna est, convallis eu maximus sed, faucibus semper velit. Sed et justo imperdiet, convallis neque nec, porttitor ante.
        Praesent ut iaculis elit. Aenean porttitor, nulla a viverra posuere, nulla neque porttitor neque, in ultrices libero tellus eget justo.
        Sed vitae arcu a dolor dictum porta sit amet ut orci. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.
      </v-col>

    </v-row>

    <v-row>
      <v-col cols="12">
        <h1>About us</h1>
      </v-col>

      <v-col cols="12" sm="6" md="4" lg="3"
        v-for="(collab, i) in collaborators"
        :key="i">

        <v-card>
          <v-img :src="collab.picture" />
          <v-card-title>{{ collab.name }}</v-card-title>
          <v-card-subtitle>{{ collab.role }}</v-card-subtitle>
          <v-card-text>{{ collab.description }}</v-card-text>
        </v-card>
      </v-col>
    </v-row>
    
  </v-container>
</template>

<script>
export default {
  name: 'About',
  data: () => ({
    collaborators: [
      {
        name: 'Patrick',
        role: 'Product Owner',
        description: 'Your product owner',
        picture: 'https://via.placeholder.com/2000'
      },
      {
        name: 'Sofian',
        role: 'Team leader',
        description: 'Your team leader',
        picture: 'https://via.placeholder.com/2000'
      },
      {
        name: 'Arthur',
        role: 'Developper',
        description: 'Rasta utopiste',
        picture: 'https://via.placeholder.com/2000'
      },
      {
        name: 'Maxence',
        role: 'Metal developper',
        description: 'Because i love metal',
        picture: 'https://via.placeholder.com/2000'
      },
      {
        name: 'Yannick',
        role: 'IOS developper',
        description: 'IOS Addict',
        picture: 'https://via.placeholder.com/2000'
      }
    ]
  }),
  computed: {
    fluid () {
      return !this.$route.name.includes('Nested')
    }
  }
}
</script>
