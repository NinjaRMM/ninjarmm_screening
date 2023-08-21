template<typename T>
bool IsInBounds(T value, T min, T max) {
  return value <= max && value >= min;
}
