namespace Raz {

template <typename T>
bool ShaderProgram::hasAttribute(const std::string& uniformName) const noexcept {
  return (hasAttribute(uniformName) && std::holds_alternative<T>(m_attributes.find(uniformName)->second));
}

template <typename T>
const T& ShaderProgram::getAttribute(const std::string& uniformName) const noexcept {
  assert("Error: The given attribute uniform name does not exist." && hasAttribute(uniformName));
  assert("Error: The fetched attribute is not of the asked type." && hasAttribute<T>(uniformName));

  return std::get<T>(m_attributes.find(uniformName)->second);
}

template <typename T>
void ShaderProgram::setAttribute(T&& attribVal, std::string uniformName) {
  m_attributes[std::move(uniformName)] = std::forward<T>(attribVal);
}

} // namespace Raz
