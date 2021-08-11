#pragma once

/// <summary>
/// Variadic sum function template
/// </summary>
template <typename T>
T sum(T t) {
	return t;
}

/// <summary>
/// Variadic sum function template
/// </summary>
template <typename T, typename... Args>
T sum(T t, Args... args) {
	return t + sum(args...);
}

/// <summary>
/// Variadic Multiply function template
/// </summary>
template <typename T>
T multiply(T t) {
	return t;
}

/// <summary>
/// Variadic Multiply function template
/// </summary>
template <typename T, typename... Args>
T multiply(T t, Args... args) {
	return t * multiply(args...);
}