#include <emscripten/val.h>
#include <stdio.h>
#include <math.h>
#include <unordered_map>

using namespace emscripten;

int main() {
  val console = val::global("console");
  console.call<void>("log", val("This is console log from val"));
  val ml = val::global("navigator")["ml"];
  val context_options = val::object();
  context_options.set("devicePreference", val("gpu"));
  context_options.set("powerPreference", val("default"));
  console.call<void>("log", ml);
  console.call<void>("log", context_options);
  std::vector<val> webnn_operands;
  webnn_operands.push_back(context_options);
  console.call<void>("log", webnn_operands[0]);
  std::unordered_map<int, val> wnn_inputs_;
  wnn_inputs_.insert(std::make_pair(0, context_options));
  console.call<void>("log", val("wnn_inputs 0: "));
  console.call<void>("log", wnn_inputs_.at(0));
  val wnn_context = ml.call<val>("createContext", context_options);

  val wnn_builder = val::global("MLGraphBuilder").new_(wnn_context);
  console.call<void>("log", wnn_context);
  console.call<void>("log", val("wnn_builder"));
  console.call<void>("log", wnn_builder);
  console.call<void>("log", wnn_builder.as<bool>());
  val AudioContext = val::global("AudioContext");

  val null_builder1 = val::null();

  console.call<void>("log", val("is null: "));
  console.call<void>("log", null_builder1.isNull());

  std::vector<int32_t> splits = {1, 2, 3, 4, 0};
  val js_splits = val::array(splits);
  val length = js_splits["length"];
  console.call<void>("log", js_splits[3]);
  printf("split length:%d\n", js_splits["length"].as<int32_t>() == 4);
  if (!wnn_builder.isUndefined()) {
      console.call<void>("log", val("wnn_builder.as<bool>() is defined."));
  }

  if (wnn_builder.as<bool>()) {
      console.call<void>("log", val("wnn_builder.as<bool>() is true."));
  }

  if (wnn_context.as<bool>()) {
      console.call<void>("log", val("wnn_context.as<bool>() is true."));
  }

  if (!AudioContext.as<bool>()) {
    printf("No global AudioContext, trying webkitAudioContext\n");
    AudioContext = val::global("webkitAudioContext");
  }

  printf("Got an AudioContext\n");
  val context = AudioContext.new_();
  val oscillator = context.call<val>("createOscillator");

  printf("Configuring oscillator\n");
  oscillator.set("type", val("triangle"));
  oscillator["frequency"].set("value", val(261.63)); // Middle C

  printf("Playing\n");
  oscillator.call<void>("connect", context["destination"]);
  oscillator.call<void>("start", 0);

  printf("All done!\n");
}