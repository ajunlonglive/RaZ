namespace Raz {

template <typename SysT>
std::size_t System::getId() {
  static_assert(std::is_base_of_v<System, SysT>, "Error: The fetched system must be derived from System.");
  static_assert(!std::is_same_v<System, SysT>, "Error: The fetched system must not be of specific type 'System'.");

  static const std::size_t id = m_maxId++;
  return id;
}

} // namespace Raz
