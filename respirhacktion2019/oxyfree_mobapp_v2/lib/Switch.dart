import 'package:flutter/material.dart';
import 'package:material_switch/material_switch.dart';

class SwitchAuto extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _SwitchState();
  }
}

class _SwitchState extends State<SwitchAuto> {
  String selectedSwitchOption = "Auto";
  List<String> switchOptions = ["Auto", "Manuel"];

  @override
  Widget build(BuildContext context) {
    return MaterialSwitch(
      padding: const EdgeInsets.only(left: 65.0, top: 10.0, bottom: 10.0),
      margin: const EdgeInsets.all(5.0),
      selectedOption: selectedSwitchOption,
      options: switchOptions,
      selectedBackgroundColor: Colors.blueGrey,
      selectedTextColor: Colors.white,
      onSelect: (String selectedOption) {
        setState(() {
          selectedSwitchOption = selectedOption;
        });
      },
    );
  }
}