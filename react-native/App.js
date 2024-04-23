import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View } from 'react-native';
import { NativeWindStyleSheet } from "nativewind";

NativeWindStyleSheet.setOutput({
  default: "native",
});

export default function App() {
  return (
    <View>
      <Text className="text-red-800 text-lg">Open up App.js to start working on your app!</Text>
      <Text className="text-red-800">Hello</Text>
      <StatusBar />
    </View>
  );
}

