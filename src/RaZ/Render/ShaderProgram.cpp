#include "RaZ/Render/Renderer.hpp"
#include "RaZ/Render/ShaderProgram.hpp"
#include "RaZ/Utils/Logger.hpp"

namespace Raz {

namespace {

inline void checkProgramUsed([[maybe_unused]] const ShaderProgram& program) {
#if defined(RAZ_CONFIG_DEBUG)
  if (!program.isUsed())
    Logger::error("The current shader program must be defined as used before sending uniforms to it.");
#endif
}

} // namespace

ShaderProgram::ShaderProgram()
  : m_index{ Renderer::createProgram() } {}

bool ShaderProgram::hasAttribute(const std::string& uniformName) const noexcept {
  return (m_attributes.find(uniformName) != m_attributes.cend());
}

bool ShaderProgram::hasTexture(const Texture& texture) const noexcept {
  return std::any_of(m_textures.cbegin(), m_textures.cend(), [&texture] (const auto& element) {
    return (element.first->getIndex() == texture.getIndex());
  });
}

bool ShaderProgram::hasTexture(const std::string& uniformName) const noexcept {
  return std::any_of(m_textures.cbegin(), m_textures.cend(), [&uniformName] (const auto& element) {
    return (element.second == uniformName);
  });
}

const Texture& ShaderProgram::getTexture(const std::string& uniformName) const {
  const auto textureIt = std::find_if(m_textures.begin(), m_textures.end(), [&uniformName] (const auto& element) {
    return (element.second == uniformName);
  });

  if (textureIt == m_textures.cend())
    throw std::invalid_argument("Error: The given attribute uniform name does not exist");

  return *textureIt->first;
}

void ShaderProgram::setTexture(TexturePtr texture, std::string uniformName) {
  const auto textureIt = std::find_if(m_textures.begin(), m_textures.end(), [&uniformName] (const auto& element) {
    return (element.second == uniformName);
  });

  if (textureIt != m_textures.end())
    textureIt->first = std::move(texture);
  else
    m_textures.emplace_back(std::move(texture), std::move(uniformName));
}

void ShaderProgram::link() const {
  Logger::debug("[ShaderProgram] Linking (ID: " + std::to_string(m_index) + ")...");
  Renderer::linkProgram(m_index);
  Logger::debug("[ShaderProgram] Linked");
}

bool ShaderProgram::isLinked() const {
  return Renderer::isProgramLinked(m_index);
}

void ShaderProgram::use() const {
  Renderer::useProgram(m_index);
}

bool ShaderProgram::isUsed() const {
  return (Renderer::getCurrentProgram() == m_index);
}

void ShaderProgram::updateShaders() const {
  Logger::debug("[ShaderProgram] Updating shaders...");

  loadShaders();
  compileShaders();
  link();
  sendAttributes();
  initTextures();

  Logger::debug("[ShaderProgram] Updated shaders");
}

void ShaderProgram::sendAttributes() const {
  if (m_attributes.empty())
    return;

  use();

  for (const auto& [name, attrib] : m_attributes)
    std::visit([this, &uniformName = name] (const auto& value) { sendUniform(uniformName, value); }, attrib);
}

void ShaderProgram::removeAttribute(const std::string& uniformName) {
  const auto attribIt = m_attributes.find(uniformName);

  if (attribIt == m_attributes.end())
    throw std::invalid_argument("Error: The given attribute uniform name does not exist");

  m_attributes.erase(attribIt);
}

void ShaderProgram::initTextures() const {
  if (m_textures.empty())
    return;

  use();

  // TODO: binding indices should be user-definable to allow the same texture to be bound to multiple uniforms
  int bindingIndex = 0;

  for (const auto& [texture, name] : m_textures)
    sendUniform(name, bindingIndex++);
}

void ShaderProgram::bindTextures() const {
  use();

  unsigned int textureIndex = 0;

  for (const auto& [texture, _] : m_textures) {
    Renderer::activateTexture(textureIndex++);
    texture->bind();
  }
}

void ShaderProgram::removeTexture(const Texture& texture) {
  m_textures.erase(std::remove_if(m_textures.begin(), m_textures.end(), [&texture] (const auto& element) {
    return (element.first->getIndex() == texture.getIndex());
  }), m_textures.end());
}

void ShaderProgram::removeTexture(const std::string& uniformName) {
  for (auto textureIt = m_textures.begin(); textureIt != m_textures.end(); ++textureIt) {
    if (textureIt->second != uniformName)
      continue;

    m_textures.erase(textureIt);
    return;
  }
}

int ShaderProgram::recoverUniformLocation(const std::string& uniformName) const {
  return Renderer::recoverUniformLocation(m_index, uniformName.c_str());
}

void ShaderProgram::sendUniform(int index, int value) const {
  checkProgramUsed(*this);
  Renderer::sendUniform(index, value);
}

void ShaderProgram::sendUniform(int index, unsigned int value) const {
  checkProgramUsed(*this);
  Renderer::sendUniform(index, value);
}

void ShaderProgram::sendUniform(int index, float value) const {
  checkProgramUsed(*this);
  Renderer::sendUniform(index, value);
}

void ShaderProgram::sendUniform(int index, const int* values, std::size_t count) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector1i(index, values, static_cast<int>(count));
}

void ShaderProgram::sendUniform(int index, const unsigned int* values, std::size_t count) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector1ui(index, values, static_cast<int>(count));
}

void ShaderProgram::sendUniform(int index, const float* values, std::size_t count) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector1(index, values, static_cast<int>(count));
}

void ShaderProgram::sendUniform(int index, const Vec2i& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector2i(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec3i& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector3i(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec4i& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector4i(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec2u& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector2ui(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec3u& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector3ui(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec4u& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector4ui(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec2f& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector2(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec3f& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector3(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Vec4f& vec) const {
  checkProgramUsed(*this);
  Renderer::sendUniformVector4(index, vec.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Mat2f& mat) const {
  checkProgramUsed(*this);
  Renderer::sendUniformMatrix2x2(index, mat.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Mat3f& mat) const {
  checkProgramUsed(*this);
  Renderer::sendUniformMatrix3x3(index, mat.getDataPtr());
}

void ShaderProgram::sendUniform(int index, const Mat4f& mat) const {
  checkProgramUsed(*this);
  Renderer::sendUniformMatrix4x4(index, mat.getDataPtr());
}

ShaderProgram::~ShaderProgram() {
  if (!m_index.isValid())
    return;

  Logger::debug("[ShaderProgram] Destroying (ID: " + std::to_string(m_index) + ")...");
  Renderer::deleteProgram(m_index);
  Logger::debug("[ShaderProgram] Destroyed");
}

void RenderShaderProgram::setVertexShader(VertexShader&& vertShader) {
  Logger::debug("[RenderShaderProgram] Setting vertex shader (ID: " + std::to_string(vertShader.getIndex()) + ", path: '" + vertShader.getPath() + "')");

  if (Renderer::isShaderAttached(m_index, m_vertShader.getIndex()))
    Renderer::detachShader(m_index, m_vertShader.getIndex());

  m_vertShader = std::move(vertShader);
  m_vertShader.compile();

  Renderer::attachShader(m_index, m_vertShader.getIndex());
}

void RenderShaderProgram::setTessellationControlShader(TessellationControlShader&& tessCtrlShader) {
  Logger::debug("[RenderShaderProgram] Setting tessellation control shader (ID: "
                + std::to_string(tessCtrlShader.getIndex()) + ", path: '" + tessCtrlShader.getPath() + "')");

  if (m_tessCtrlShader && Renderer::isShaderAttached(m_index, m_tessCtrlShader->getIndex()))
    Renderer::detachShader(m_index, m_tessCtrlShader->getIndex());

  m_tessCtrlShader = std::move(tessCtrlShader);
  m_tessCtrlShader->compile();

  Renderer::attachShader(m_index, m_tessCtrlShader->getIndex());
}

void RenderShaderProgram::setTessellationEvaluationShader(TessellationEvaluationShader&& tessEvalShader) {
  Logger::debug("[RenderShaderProgram] Setting tessellation evaluation shader (ID: "
                + std::to_string(tessEvalShader.getIndex()) + ", path: '" + tessEvalShader.getPath() + "')");

  if (m_tessEvalShader && Renderer::isShaderAttached(m_index, m_tessEvalShader->getIndex()))
    Renderer::detachShader(m_index, m_tessEvalShader->getIndex());

  m_tessEvalShader = std::move(tessEvalShader);
  m_tessEvalShader->compile();

  Renderer::attachShader(m_index, m_tessEvalShader->getIndex());
}

void RenderShaderProgram::setGeometryShader(GeometryShader&& geomShader) {
  Logger::debug("[RenderShaderProgram] Setting geometry shader (ID: " + std::to_string(geomShader.getIndex()) + ", path: '" + geomShader.getPath() + "')");

  if (m_geomShader && Renderer::isShaderAttached(m_index, m_geomShader->getIndex()))
    Renderer::detachShader(m_index, m_geomShader->getIndex());

  m_geomShader = std::move(geomShader);
  m_geomShader->compile();

  Renderer::attachShader(m_index, m_geomShader->getIndex());
}

void RenderShaderProgram::setFragmentShader(FragmentShader&& fragShader) {
  Logger::debug("[RenderShaderProgram] Setting fragment shader (ID: " + std::to_string(fragShader.getIndex()) + ", path: '" + fragShader.getPath() + "')");

  if (Renderer::isShaderAttached(m_index, m_fragShader.getIndex()))
    Renderer::detachShader(m_index, m_fragShader.getIndex());

  m_fragShader = std::move(fragShader);
  m_fragShader.compile();

  Renderer::attachShader(m_index, m_fragShader.getIndex());
}

void RenderShaderProgram::setShaders(VertexShader&& vertShader, FragmentShader&& fragShader) {
  setVertexShader(std::move(vertShader));
  setFragmentShader(std::move(fragShader));

  link();
}

void RenderShaderProgram::setShaders(VertexShader&& vertShader, GeometryShader&& geomShader, FragmentShader&& fragShader) {
  setVertexShader(std::move(vertShader));
  setGeometryShader(std::move(geomShader));
  setFragmentShader(std::move(fragShader));

  link();
}

void RenderShaderProgram::setShaders(VertexShader&& vertShader, TessellationEvaluationShader&& tessEvalShader, FragmentShader&& fragShader) {
  setVertexShader(std::move(vertShader));
  setTessellationEvaluationShader(std::move(tessEvalShader));
  setFragmentShader(std::move(fragShader));

  link();
}
void RenderShaderProgram::setShaders(VertexShader&& vertShader,
                                     TessellationControlShader&& tessCtrlShader,
                                     TessellationEvaluationShader&& tessEvalShader,
                                     FragmentShader&& fragShader) {
  setVertexShader(std::move(vertShader));
  setTessellationControlShader(std::move(tessCtrlShader));
  setTessellationEvaluationShader(std::move(tessEvalShader));
  setFragmentShader(std::move(fragShader));

  link();
}

RenderShaderProgram RenderShaderProgram::clone() const {
  RenderShaderProgram program;

  program.setVertexShader(m_vertShader.clone());
  if (m_tessCtrlShader) program.setTessellationControlShader(m_tessCtrlShader->clone());
  if (m_tessEvalShader) program.setTessellationEvaluationShader(m_tessEvalShader->clone());
  if (m_geomShader) program.setGeometryShader(m_geomShader->clone());
  program.setFragmentShader(m_fragShader.clone());

  program.link();

  program.m_attributes = m_attributes;
  program.m_textures   = m_textures;

  sendAttributes();
  initTextures();

  return program;
}

void RenderShaderProgram::loadShaders() const {
  Logger::debug("[RenderShaderProgram] Loading shaders...");

  m_vertShader.load();
  if (m_tessCtrlShader) m_tessCtrlShader->load();
  if (m_tessEvalShader) m_tessEvalShader->load();
  if (m_geomShader) m_geomShader->load();
  m_fragShader.load();

  Logger::debug("[RenderShaderProgram] Loaded shaders");
}

void RenderShaderProgram::compileShaders() const {
  Logger::debug("[RenderShaderProgram] Compiling shaders...");

  m_vertShader.compile();
  if (m_tessCtrlShader) m_tessCtrlShader->compile();
  if (m_tessEvalShader) m_tessEvalShader->compile();
  if (m_geomShader) m_geomShader->compile();
  m_fragShader.compile();

  Logger::debug("[RenderShaderProgram] Compiled shaders");
}

void RenderShaderProgram::destroyVertexShader() {
  Renderer::detachShader(m_index, m_vertShader.getIndex());
  m_vertShader.destroy();
}

void RenderShaderProgram::destroyTessellationControlShader() {
  if (!m_tessCtrlShader)
    return;

  Renderer::detachShader(m_index, m_tessCtrlShader->getIndex());
  m_tessCtrlShader->destroy();
  m_tessCtrlShader.reset();
}

void RenderShaderProgram::destroyTessellationEvaluationShader() {
  if (!m_tessEvalShader)
    return;

  Renderer::detachShader(m_index, m_tessEvalShader->getIndex());
  m_tessEvalShader->destroy();
  m_tessEvalShader.reset();
}

void RenderShaderProgram::destroyGeometryShader() {
  if (!m_geomShader)
    return;

  Renderer::detachShader(m_index, m_geomShader->getIndex());
  m_geomShader->destroy();
  m_geomShader.reset();
}

void RenderShaderProgram::destroyFragmentShader() {
  Renderer::detachShader(m_index, m_fragShader.getIndex());
  m_fragShader.destroy();
}

void ComputeShaderProgram::setShader(ComputeShader&& compShader) {
  Logger::debug("[ComputeShaderProgram] Setting shader (ID: " + std::to_string(compShader.getIndex()) + ", path: '" + compShader.getPath() + "')");

  if (Renderer::isShaderAttached(m_index, m_compShader.getIndex()))
    Renderer::detachShader(m_index, m_compShader.getIndex());

  m_compShader = std::move(compShader);
  m_compShader.compile();

  Renderer::attachShader(m_index, m_compShader.getIndex());

  link();
}

ComputeShaderProgram ComputeShaderProgram::clone() const {
  ComputeShaderProgram program;

  program.setShader(m_compShader.clone());

  program.m_attributes = m_attributes;
  program.m_textures   = m_textures;

  sendAttributes();
  initTextures();

  return program;
}

void ComputeShaderProgram::loadShaders() const {
  Logger::debug("[ComputeShaderProgram] Loading shader...");
  m_compShader.load();
  Logger::debug("[ComputeShaderProgram] Loaded shader");
}

void ComputeShaderProgram::compileShaders() const {
  Logger::debug("[ComputeShaderProgram] Compiling shader...");
  m_compShader.compile();
  Logger::debug("[ComputeShaderProgram] Compiled shader");
}

void ComputeShaderProgram::execute(unsigned int groupCountX, unsigned int groupCountY, unsigned int groupCountZ) const {
  use();
  Renderer::dispatchCompute(groupCountX, groupCountY, groupCountZ);
  Renderer::setMemoryBarrier(BarrierType::ALL);
}

void ComputeShaderProgram::destroyShader() {
  Renderer::detachShader(m_index, m_compShader.getIndex());
  m_compShader.destroy();
}

} // namespace Raz
