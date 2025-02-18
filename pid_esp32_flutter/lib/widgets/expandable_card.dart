import 'package:flutter/material.dart';

class ExpandableCard extends StatelessWidget {
  final String title;
  final bool isVisible;
  final VoidCallback onToggle;
  final Widget child;

  const ExpandableCard({
    Key? key,
    required this.title,
    required this.isVisible,
    required this.onToggle,
    required this.child,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: onToggle,
      child: Container(
        decoration: BoxDecoration(
          border: Border.all(color: Colors.white, width: 1.5),
          color: Colors.white.withOpacity(0.2),
          borderRadius: BorderRadius.circular(10),
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            ListTile(
              contentPadding: EdgeInsets.only(left: 10),
              title: Text(
                title,
                style: TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                  color: Colors.white70,
                ),
              ),
              trailing: IconButton(
                icon: Icon(
                  isVisible ? Icons.keyboard_arrow_right : Icons.keyboard_arrow_down,
                  color: Colors.white,
                  size: 30,
                ),
                onPressed: onToggle,
              ),
            ),
            if (isVisible) Padding(padding: const EdgeInsets.all(10), child: child),
          ],
        ),
      ),
    );
  }
}
