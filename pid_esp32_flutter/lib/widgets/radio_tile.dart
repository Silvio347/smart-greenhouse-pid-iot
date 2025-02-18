import 'package:flutter/material.dart';

class CustomRadioTile extends StatelessWidget {
  final String title;
  final int value;
  final int groupValue;
  final ValueChanged<int?> onChanged;

  const CustomRadioTile({
    Key? key,
    required this.title,
    required this.value,
    required this.groupValue,
    required this.onChanged,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return RadioListTile<int>(
      activeColor: Colors.white,
      title: Text(
        title,
        style: TextStyle(color: Colors.white),
      ),
      value: value,
      groupValue: groupValue,
      contentPadding: EdgeInsets.zero,
      onChanged: onChanged,
    );
  }
}
