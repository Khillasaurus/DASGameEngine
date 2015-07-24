<font face = "Consolas">
# DASGameEngine (DASGE) - Render System Logic Flow  
---
Preface: The render system is based on using data from the following two classes to render everything:  
- ModelAsset  
- ModelInstance  
*See ModelAsset.h in order to understand the layout of Models.*  
*See ModelInstance.h in order to understand how instances utilize data from their asset counterparts.*  
- It is important to note that ModelInstance.h is dependent on ModelAsset.h, but the ModelInstance class does **NOT** inherit from anything.  
- Objects *can* hold a ModelAsset pointer, but are not required to, thus separating rendering logic from object interaction logic.


---
## Application.h + Application.cpp  
- **[Background]**  
 - **[Clear the screen to black]**  
&nbsp;&nbsp;&nbsp;&nbsp;`glClearColor(0.0f, 0.0f, 0.0f, 1.0f);`  
&nbsp;&nbsp;&nbsp;&nbsp;`glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);`  
 - **[Iterate through Instance Lists]**  
&nbsp;&nbsp;&nbsp;&nbsp;`std::vector<DSGraphics::ModelInstance>::iterator it;`  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Abstracts]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`it->Render();`&nbsp;**[see next section (ModelInstance.cpp)for more information]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Aesthetics]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`it->Render();`&nbsp;**[see next section (ModelInstance.cpp)for more information]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Environmentals]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`it->Render();`&nbsp;**[see next section (ModelInstance.cpp)for more information]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Player]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`it->Render();`&nbsp;**[see next section (ModelInstance.cpp)for more information]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Units]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`it->Render();`&nbsp;**[see next section (ModelInstance.cpp)for more information]**  
## ModelInstance.h + ModelInstance.cpp  
### Note: See code for safety checks, logic branches, and other additional information.  
- *Render()*  
 - **[Bind]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Shaders]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glUseProgram(mpAsset->GetProgramID());`  
 - **[Set the Shader Uniforms]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Camera]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`mUniformCamera = glGetUniformLocation(mpAsset->GetProgramID(), "camera");`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glUniformMatrix4fv(mUniformCamera, 1, GL_FALSE, glm::value_ptr(mpCamera->GetMatrix()));`  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Model]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`mUniformModel = glGetUniformLocation(mpAsset->GetProgramID(), "model");`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(mTransform));`  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Texture]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**[If the asset has a texture...]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glUniform1i(glGetUniformLocation(mpAsset->GetProgramID(), "tex"), 0);`  
 - **[Continue Binding]**  
&nbsp;&nbsp;&nbsp;&nbsp;- **[Texture (based on above if has texture check)]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**[Choose a texture]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**[Set chosen texture to "active"]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glActiveTexture(`**[chosen texture]**`);`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**[Bind active texture]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glBindTexture(GL_TEXTURE_2D, mpAsset->GetTextureObjectID());`  
&nbsp;&nbsp;&nbsp;&nbsp;- **[VAO]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`glBindVertexArray(mpAsset->GetVao());`  
- **[Draw]**  
 - **[Elements]**  
&nbsp;&nbsp;&nbsp;&nbsp;**[If: the asset uses elements to render reduce model data...]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**- [Poll element data]**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**- [If ...]**  
&nbsp;&nbsp;&nbsp;&nbsp;**[Else: the asset uses arrays to render model data...]**  






## Application.h + Application.cpp  
- **[Swap the back buffer and the front buffer]**
</font>