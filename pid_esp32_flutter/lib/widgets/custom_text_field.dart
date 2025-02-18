import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class CustomTextField extends StatelessWidget {
  final TextEditingController controller;
  final String labelText;
  final String? errorText;
  final TextInputType keyboardType;
  final TextInputFormatter? inputFormatter;
  final Function(String)? onChanged;

  CustomTextField({
    required this.controller,
    required this.labelText,
    this.errorText,
    required this.keyboardType,
    this.inputFormatter,
    this.onChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        TextField(
          controller: controller,
          decoration: InputDecoration(
            contentPadding:
                EdgeInsets.symmetric(vertical: 8.0, horizontal: 12.0),
            errorText: errorText,
            errorStyle: TextStyle(
              fontSize: 12,
              fontWeight: FontWeight.normal,
              overflow: TextOverflow.ellipsis,
              color: Colors.red,
            ),
            errorMaxLines: 2,
            errorBorder: OutlineInputBorder(
              borderSide: BorderSide(
                color: Colors.red,
                width: 2,
              ),
            ),
            labelText: labelText,
            labelStyle: TextStyle(color: Colors.white),
            focusedBorder: OutlineInputBorder(
              borderSide: BorderSide(
                color: Colors.white,
                width: 2.0,
              ),
            ),
            enabledBorder: OutlineInputBorder(
              borderSide: BorderSide(
                color: Colors.white,
                width: 2.0,
              ),
            ),
          ),
          style: TextStyle(color: Colors.white),
          keyboardType: keyboardType,
          cursorColor: Colors.white,
          inputFormatters: inputFormatter != null ? [inputFormatter!] : null,
          onChanged:
              onChanged, // Call the optional onChanged callback if provided
        ),
        SizedBox(
          height: 0.01 * MediaQuery.of(context).size.height,
        ),
      ],
    );
  }
}
