/*
 ------------------------------------------------------------------------------
 Copyright (C) 1997-2001 Id Software.

 This file is part of the Quake 2 source code.

 The Quake 2 source code is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 The Quake 2 source code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 the Quake 2 source code; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 ------------------------------------------------------------------------------
*/


//
// r_backEnd.c - Back-end renderer
//


#include "r_local.h"


backEnd_t					backEnd;


/*
 ==============================================================================

 TEXTURE TOOLS

 ==============================================================================
*/


/*
 ==================
 RB_TestTexture

 Displays a single texture over most of the screen
 ==================
*/
static void RB_TestTexture (){

	mat4_t	projectionMatrix = {2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f};
	float	w, h;
	int		max;

	if (!rg.testTexture)
		return;

	max = Max(rg.testTexture->width, rg.testTexture->height);

	w = 0.25f * rg.testTexture->width / max;
	h = 0.25f * rg.testTexture->height / max;

	w *= (float)backEnd.cropHeight / backEnd.cropWidth;

	// Set the GL state
	GL_LoadMatrix(GL_PROJECTION, projectionMatrix);
	GL_LoadIdentity(GL_MODELVIEW);

	GL_PolygonMode(GL_FILL);

	GL_Disable(GL_CULL_FACE);

	GL_Disable(GL_POLYGON_OFFSET_FILL);

	GL_Enable(GL_BLEND);
	GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL_BlendEquation(GL_FUNC_ADD);

	GL_Disable(GL_ALPHA_TEST);
	GL_Disable(GL_DEPTH_TEST);
	GL_Disable(GL_STENCIL_TEST);

	GL_DepthRange(0.0f, 1.0f);

	GL_ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL_DepthMask(GL_FALSE);
	GL_StencilMask(255);

	// Set up the texture
	GL_EnableTexture(rg.testTexture->target);
	GL_BindTexture(rg.testTexture);

	GL_LoadIdentity(GL_TEXTURE);
	GL_TexEnv(GL_REPLACE);

	// If a cube map texture
	if (rg.testTexture->type == TT_CUBE){
		w *= 0.333333f;
		h *= 0.333333f;

		// Draw it
		qglBegin(GL_QUADS);
		qglTexCoord3f(-1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f - w * 4.0f, h * 4.0f);
		qglTexCoord3f(-1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 4.0f);
		qglTexCoord3f(-1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 2.0f);
		qglTexCoord3f(-1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f - w * 4.0f, h * 2.0f);
		qglTexCoord3f(-1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 4.0f);
		qglTexCoord3f( 1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f, h * 4.0f);
		qglTexCoord3f( 1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f, h * 2.0f);
		qglTexCoord3f(-1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 2.0f);
		qglTexCoord3f( 1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f, h * 4.0f);
		qglTexCoord3f( 1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f + w * 2.0f, h * 4.0f);
		qglTexCoord3f( 1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f + w * 2.0f, h * 2.0f);
		qglTexCoord3f( 1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f, h * 2.0f);
		qglTexCoord3f( 1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f + w * 2.0f, h * 4.0f);
		qglTexCoord3f(-1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f + w * 4.0f, h * 4.0f);
		qglTexCoord3f(-1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f + w * 4.0f, h * 2.0f);
		qglTexCoord3f( 1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f + w * 2.0f, h * 2.0f);
		qglTexCoord3f(-1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 6.0f);
		qglTexCoord3f( 1.0f,  1.0f, -1.0f);
		qglVertex2f(0.5f, h * 6.0f);
		qglTexCoord3f( 1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f, h * 4.0f);
		qglTexCoord3f(-1.0f,  1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 4.0f);
		qglTexCoord3f(-1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f - w * 2.0f, h * 2.0f);
		qglTexCoord3f( 1.0f, -1.0f,  1.0f);
		qglVertex2f(0.5f, h * 2.0f);
		qglTexCoord3f( 1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f, 0.0f);
		qglTexCoord3f(-1.0f, -1.0f, -1.0f);
		qglVertex2f(0.5f - w * 2.0f, 0.0f);
		qglEnd();

		// Check for errors
		if (!r_ignoreGLErrors->integerValue)
			GL_CheckForErrors();

		return;
	}

	// Draw it
	qglBegin(GL_QUADS);
	qglTexCoord2f(0.0f, 0.0f);
	qglVertex2f(0.5f - w, h * 2.0f);
	qglTexCoord2f(1.0f, 0.0f);
	qglVertex2f(0.5f + w, h * 2.0f);
	qglTexCoord2f(1.0f, 1.0f);
	qglVertex2f(0.5f + w, 0.0f);
	qglTexCoord2f(0.0f, 1.0f);
	qglVertex2f(0.5f - w, 0.0f);
	qglEnd();

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();
}

/*
 ==================
 RB_ShowTextures

 Draws all the textures to the screen, on top of whatever was there.
 This is used to test for texture trashing.
 ==================
*/
static void RB_ShowTextures (){

	texture_t	*texture;
	float		x, y, w, h;
	int			time;
	int			i;

	if (!r_showTextures->integerValue)
		return;

	// Switch to 2D mode
	GL_Setup2D(rg.time);

	qglFinish();

	time = Sys_Milliseconds();

	// Draw all textures
	for (i = 0; i < MAX_TEXTURES; i++){
		texture = R_GetTextureByIndex(i);
		if (!texture)
			break;

		w = backEnd.cropWidth / 20;
		h = backEnd.cropHeight / 15;

		x = i % 20 * w;
		y = i / 20 * h;

		// Draw in proportional size if desired
		if (r_showTextures->integerValue == 2){
			w *= texture->width / 512.0f;
			h *= texture->height / 512.0f;
		}

		// Set up the texture
		GL_EnableTexture(texture->target);
		GL_BindTexture(texture);

		GL_LoadIdentity(GL_TEXTURE);
		GL_TexEnv(GL_REPLACE);

		// Draw it
		qglBegin(GL_QUADS);
		qglTexCoord2f(0.0f, 0.0f);
		qglVertex2f(x, y);
		qglTexCoord2f(1.0f, 0.0f);
		qglVertex2f(x + w, y);
		qglTexCoord2f(1.0f, 1.0f);
		qglVertex2f(x + w, y + h);
		qglTexCoord2f(0.0f, 1.0f);
		qglVertex2f(x, y + h);
		qglEnd();
	}

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	// Force a mode switch
	backEnd.projection2D = false;

	qglFinish();

	Com_Printf("%i msec to draw all textures\n", Sys_Milliseconds() - time);
}


/*
 ==============================================================================

 RENDER COMMANDS EXECUTION

 ==============================================================================
*/


/*
 ==================
 RB_RenderView
 ==================
*/
static const void *RB_RenderView (const void *data){

	const renderViewCommand_t	*cmd = (const renderViewCommand_t *)data;

	// Development tool
	if (r_skipRender->integerValue)
		return (const void *)(cmd + 1);

	// Finish 2D rendering if needed
	if (backEnd.projection2D)
		RB_RenderBatch();

	QGL_LogPrintf("---------- RB_RenderView ----------\n");

	rg.pc.views++;

	backEnd.viewParms = cmd->viewParms;

	// Development tool
	if (r_skipRenderContext->integerValue)
		GLImp_ActivateContext(false);

	// Switch to 3D projection
	GL_Setup3D(cmd->time);

	// Set up the scissor
	GL_Scissor(backEnd.scissor);

	// Z-Fill pass
	RB_FillDepthBuffer(cmd->viewParms.numMeshes[0], cmd->viewParms.meshes[0]);

	// Shadow and interaction pass
	RB_RenderLights(cmd->viewParms.numLights[0], cmd->viewParms.lights[0]);

	// Ambient pass (opaque)
	RB_RenderMaterialPasses(cmd->viewParms.numMeshes[0], cmd->viewParms.meshes[0], AP_OPAQUE);

	// Blend light pass
	RB_RenderBlendLights(cmd->viewParms.numLights[1], cmd->viewParms.lights[1]);

	// Ambient pass (translucent)
	RB_RenderMaterialPasses(cmd->viewParms.numMeshes[1], cmd->viewParms.meshes[1], AP_TRANSLUCENT);

	// Fog light pass (view outside volume)
	RB_RenderFogLights(cmd->viewParms.numLights[2], cmd->viewParms.lights[2]);

	// Ambient pass (translucent)
	RB_RenderMaterialPasses(cmd->viewParms.numMeshes[2], cmd->viewParms.meshes[2], AP_TRANSLUCENT);

	// Fog light pass (view inside volume)
	RB_RenderFogLights(cmd->viewParms.numLights[3], cmd->viewParms.lights[3]);

	// Ambient pass (post-process)
	RB_RenderMaterialPasses(cmd->viewParms.numMeshes[3], cmd->viewParms.meshes[3], AP_POST_PROCESS);

	// Bloom and color correction post-processing
	RB_PostProcess(&cmd->postProcessParms);

	// Debug tools visualization
	RB_RenderDebugTools();

	// Development tool
	if (r_skipRenderContext->integerValue){
		GLImp_ActivateContext(true);

		GL_SetDefaultState();
	}

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_CaptureRender
 ==================
*/
static const void *RB_CaptureRender (const void *data){

	const captureRenderCommand_t	*cmd = (const captureRenderCommand_t *)data;

	// Finish 2D rendering if needed
	if (backEnd.projection2D)
		RB_RenderBatch();

	QGL_LogPrintf("---------- RB_CaptureRender ----------\n");

	// Update the texture
	R_CopyFramebufferToTexture(cmd->texture, 0, backEnd.viewport.x, backEnd.viewport.y, backEnd.viewport.width, backEnd.viewport.height);

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_UpdateTexture
 ==================
*/
static const void *RB_UpdateTexture (const void *data){

	const updateTextureCommand_t	*cmd = (const updateTextureCommand_t *)data;

	// Finish 2D rendering if needed
	if (backEnd.projection2D)
		RB_RenderBatch();

	QGL_LogPrintf("---------- RB_UpdateTexture ----------\n");

	// Update the texture
	R_UploadTextureImage(cmd->texture, 0, cmd->image, cmd->width, cmd->height);

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_SetColor
 ==================
*/
static const void *RB_SetColor (const void *data){

	const setColorCommand_t	*cmd = (const setColorCommand_t *)data;

	MakeRGBA(backEnd.color2D, cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]);

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_SetParameter
 ==================
*/
static const void *RB_SetParameter (const void *data){

	const setParameterCommand_t		*cmd = (const setParameterCommand_t *)data;

	backEnd.parms2D[cmd->index] = cmd->value;

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_SetParameters
 ==================
*/
static const void *RB_SetParameters (const void *data){

	const setParametersCommand_t	*cmd = (const setParametersCommand_t *)data;

	Mem_Copy(backEnd.parms2D, cmd->parms, MAX_MATERIAL_PARMS * sizeof(float));

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_DrawStretchPic
 ==================
*/
static const void *RB_DrawStretchPic (const void *data){

	const drawPicStretchCommand_t	*cmd = (const drawPicStretchCommand_t *)data;
	glIndex_t						*indices;
	glVertex_t						*vertices;
	int								i;

	// Switch to 2D mode if needed
	if (!backEnd.projection2D){
		GL_Setup2D(rg.time);

		// Clear the batch state
		backEnd.entity = NULL;
		backEnd.material = NULL;

		backEnd.stencilShadow = false;
		backEnd.shadowCaps = false;
	}

	// Check if the state changed
	if (cmd->material != backEnd.material){
		// Draw the last batch
		RB_RenderBatch();

		// Evaluate registers
		RB_EvaluateRegisters(cmd->material, backEnd.floatTime, backEnd.parms2D);

		// Create a new batch
		RB_SetupBatch(rg.worldEntity, cmd->material, false, false, RB_DrawMaterial2D);
	}

	// Skip if condition evaluated to false
	if (!cmd->material->expressionRegisters[cmd->material->conditionRegister])
		return (const void *)(cmd + 1);

	// Check for overflow
	RB_CheckMeshOverflow(6, 4);

	// Batch indices
	indices = backEnd.indices + backEnd.numIndices;

	indices[0] = backEnd.numVertices;
	indices[1] = backEnd.numVertices + 1;
	indices[2] = backEnd.numVertices + 3;
	indices[3] = backEnd.numVertices + 3;
	indices[4] = backEnd.numVertices + 1;
	indices[5] = backEnd.numVertices + 2;

	backEnd.numIndices += 6;

	// Batch vertices
	vertices = backEnd.vertices + backEnd.numVertices;

	vertices[0].xyz[0] = cmd->x1;
	vertices[0].xyz[1] = cmd->y1;
	vertices[0].xyz[2] = 0.0f;
	vertices[1].xyz[0] = cmd->x2;
	vertices[1].xyz[1] = cmd->y2;
	vertices[1].xyz[2] = 0.0f;
	vertices[2].xyz[0] = cmd->x3;
	vertices[2].xyz[1] = cmd->y3;
	vertices[2].xyz[2] = 0.0f;
	vertices[3].xyz[0] = cmd->x4;
	vertices[3].xyz[1] = cmd->y4;
	vertices[3].xyz[2] = 0.0f;

	vertices[0].st[0] = cmd->s1;
	vertices[0].st[1] = cmd->t1;
	vertices[1].st[0] = cmd->s2;
	vertices[1].st[1] = cmd->t1;
	vertices[2].st[0] = cmd->s2;
	vertices[2].st[1] = cmd->t2;
	vertices[3].st[0] = cmd->s1;
	vertices[3].st[1] = cmd->t2;

	for (i = 0; i < 4; i++){
		vertices->color[0] = backEnd.color2D[0];
		vertices->color[1] = backEnd.color2D[1];
		vertices->color[2] = backEnd.color2D[2];
		vertices->color[3] = backEnd.color2D[3];

		vertices++;
	}

	backEnd.numVertices += 4;

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_DrawCinematic
 ==================
*/
static const void *RB_DrawCinematic (const void *data){

	const drawCinematicCommand_t	*cmd = (const drawCinematicCommand_t *)data;

	// Finish 2D rendering if needed
	if (backEnd.projection2D)
		RB_RenderBatch();

	// Switch to 2D mode if needed
	if (!backEnd.projection2D){
		GL_Setup2D(rg.time);

		// Clear the batch state
		backEnd.entity = NULL;
		backEnd.material = NULL;

		backEnd.stencilShadow = false;
		backEnd.shadowCaps = false;
	}

	QGL_LogPrintf("---------- RB_DrawCinematic ----------\n");

	// Unbind the index buffer
	GL_BindIndexBuffer(NULL);

	// Unbind the vertex buffer
	GL_BindVertexBuffer(NULL);

	// Set the GL state
	GL_PolygonMode(GL_FILL);

	GL_Disable(GL_CULL_FACE);
	GL_Disable(GL_POLYGON_OFFSET_FILL);
	GL_Disable(GL_BLEND);
	GL_Disable(GL_ALPHA_TEST);
	GL_Disable(GL_DEPTH_TEST);
	GL_Disable(GL_STENCIL_TEST);

	GL_DepthRange(0.0f, 1.0f);

	GL_ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL_DepthMask(GL_FALSE);
	GL_StencilMask(255);

	// Draw vertical black bars if needed
	if (cmd->vx1 != cmd->wx1 || cmd->vx2 != cmd->wx2){
		GL_DisableTexture();

		qglColor3f(0.0f, 0.0f, 0.0f);

		qglBegin(GL_QUADS);
		qglVertex2i(cmd->vx1, cmd->wy1);
		qglVertex2i(cmd->vx1, cmd->wy2);
		qglVertex2i(cmd->wx1, cmd->wy2);
		qglVertex2i(cmd->wx1, cmd->wy1);
		qglVertex2i(cmd->wx2, cmd->wy1);
		qglVertex2i(cmd->wx2, cmd->wy2);
		qglVertex2i(cmd->vx2, cmd->wy2);
		qglVertex2i(cmd->vx2, cmd->wy1);
		qglEnd();
	}

	// Draw horizontal black bars if needed
	if (cmd->vy1 != cmd->wy1 || cmd->vy2 != cmd->wy2){
		GL_DisableTexture();

		qglColor3f(0.0f, 0.0f, 0.0f);

		qglBegin(GL_QUADS);
		qglVertex2i(cmd->wx1, cmd->wy1);
		qglVertex2i(cmd->wx2, cmd->wy1);
		qglVertex2i(cmd->wx2, cmd->vy1);
		qglVertex2i(cmd->wx1, cmd->vy1);
		qglVertex2i(cmd->wx1, cmd->vy2);
		qglVertex2i(cmd->wx2, cmd->vy2);
		qglVertex2i(cmd->wx2, cmd->wy2);
		qglVertex2i(cmd->wx1, cmd->wy2);
		qglEnd();
	}

	// Set up the texture
	GL_EnableTexture(cmd->texture->target);
	GL_BindTexture(cmd->texture);

	GL_LoadIdentity(GL_TEXTURE);
	GL_TexEnv(GL_REPLACE);

	// Update the texture if needed
	if (cmd->dirty)
		R_UploadTextureImage(cmd->texture, 0, cmd->image, cmd->width, cmd->height);

	// Draw it
	qglBegin(GL_QUADS);
	qglTexCoord2f(0.0f, 0.0f);
	qglVertex2i(cmd->vx1, cmd->vy1);
	qglTexCoord2f(1.0f, 0.0f);
	qglVertex2i(cmd->vx2, cmd->vy1);
	qglTexCoord2f(1.0f, 1.0f);
	qglVertex2i(cmd->vx2, cmd->vy2);
	qglTexCoord2f(0.0f, 1.0f);
	qglVertex2i(cmd->vx1, cmd->vy2);
	qglEnd();

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_CropSize
 ==================
*/
static const void *RB_CropSize (const void *data){

	const cropSizeCommand_t	*cmd = (const cropSizeCommand_t *)data;

	// Finish 2D rendering if needed and force a mode switch
	if (backEnd.projection2D){
		RB_RenderBatch();

		backEnd.projection2D = false;
	}

	// Set the current crop size
	backEnd.cropWidth = cmd->width;
	backEnd.cropHeight = cmd->height;

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_SetupBuffers
 ==================
*/
static const void *RB_SetupBuffers (const void *data){

	const setupBuffersCommand_t	*cmd = (const setupBuffersCommand_t *)data;
	int							index;

	// Finish 2D rendering if needed and force a mode switch
	if (backEnd.projection2D){
		RB_RenderBatch();

		backEnd.projection2D = false;
	}

	QGL_LogPrintf("---------- RB_SetupBuffers ----------\n");

	// Set the draw and read buffers
	if (r_frontBuffer->integerValue){
		qglDrawBuffer(GL_FRONT);
		qglReadBuffer(GL_FRONT);
	}
	else {
		qglDrawBuffer(GL_BACK);
		qglReadBuffer(GL_BACK);
	}

	// Clear the color buffer if desired
	if (r_clear->integerValue){
		index = r_clearColor->integerValue & COLOR_MASK;

		GL_ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		qglClearColor(color_table[index][0], color_table[index][1], color_table[index][2], color_table[index][3]);
		qglClear(GL_COLOR_BUFFER_BIT);
	}

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_SwapBuffers
 ==================
*/
static const void *RB_SwapBuffers (const void *data){

	const swapBuffersCommand_t	*cmd = (const swapBuffersCommand_t *)data;

	// Finish 2D rendering if needed and force a mode switch
	if (backEnd.projection2D){
		RB_RenderBatch();

		backEnd.projection2D = false;
	}

	QGL_LogPrintf("---------- RB_SwapBuffers ----------\n");

	// Development tools
	RB_TestTexture();

	RB_ShowTextures();

	// Swap the buffers
	if (r_frontBuffer->integerValue || r_finish->integerValue)
		qglFinish();

	if (!r_frontBuffer->integerValue)
		GLImp_SwapBuffers();

	// Check for errors
	if (!r_ignoreGLErrors->integerValue)
		GL_CheckForErrors();

	QGL_LogPrintf("--------------------\n\n\n");

	return (const void *)(cmd + 1);
}

/*
 ==================
 RB_ExecuteRenderCommands
 ==================
*/
void RB_ExecuteRenderCommands (const void *data){

	renderCommand_t	commandId;
	int				timeBackEnd;

	if (r_skipBackEnd->integerValue)
		return;

	if (com_speeds->integerValue)
		timeBackEnd = Sys_Milliseconds();

	while (1){
		commandId = *(const renderCommand_t *)data;

		if (commandId == RC_END_OF_LIST){
			if (com_speeds->integerValue)
				com_timeBackEnd += (Sys_Milliseconds() - timeBackEnd);

			return;
		}

		switch (commandId){
		case RC_RENDER_VIEW:
			data = RB_RenderView(data);
			break;
		case RC_CAPTURE_RENDER:
			data = RB_CaptureRender(data);
			break;
		case RC_UPDATE_TEXTURE:
			data = RB_UpdateTexture(data);
			break;
		case RC_SET_COLOR:
			data = RB_SetColor(data);
			break;
		case RC_SET_PARAMETER:
			data = RB_SetParameter(data);
			break;
		case RC_SET_PARAMETERS:
			data = RB_SetParameters(data);
			break;
		case RC_DRAW_STRETCH_PIC:
			data = RB_DrawStretchPic(data);
			break;
		case RC_DRAW_CINEMATIC:
			data = RB_DrawCinematic(data);
			break;
		case RC_CROP_SIZE:
			data = RB_CropSize(data);
			break;
		case RC_SETUP_BUFFERS:
			data = RB_SetupBuffers(data);
			break;
		case RC_SWAP_BUFFERS:
			data = RB_SwapBuffers(data);
			break;
		default:
			Com_Error(ERR_DROP, "RB_ExecuteRenderCommands: bad command id (%i)", commandId);
		}
	}
}


/*
 ==============================================================================

 SHADERS AND PROGRAMS SETUP

 ==============================================================================
*/


/*
 ==================
 RB_SetupInteractionShaders

 FIXME: projection and directional shaders cannot find u_lightPlane for some reason...
 ==================
*/
static void RB_SetupInteractionShaders (){

	shader_t	*vertexShader, *fragmentShader;

	// Load pointGeneric
	vertexShader = R_FindShader("interaction/pointGeneric", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("interaction/pointGeneric", GL_FRAGMENT_SHADER);

	rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT] = R_FindProgram("interaction/pointGeneric", vertexShader, fragmentShader);
	if (!rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT])
		Com_Error(ERR_FATAL, "RB_SetupInteractionShaders: invalid program '%s'", "interaction/pointGeneric");

	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].viewOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_ViewOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].lightOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_LightOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].bumpMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_BumpMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].diffuseMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_DiffuseMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].specularMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_SpecularMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].lightMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].colorScaleAndBias = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_ColorScaleAndBias", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].diffuseColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_DiffuseColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].specularColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_SpecularColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].specularParms = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_SpecularParms", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_POINT].lightColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_LightColor", 1, GL_FLOAT_VEC3);

	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_BumpMap", 1, GL_SAMPLER_2D, TMU_BUMP);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_DiffuseMap", 1, GL_SAMPLER_2D, TMU_DIFFUSE);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_SpecularMap", 1, GL_SAMPLER_2D, TMU_SPECULAR);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_POINT], "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);

	// Load cubicGeneric
	vertexShader = R_FindShader("interaction/cubicGeneric", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("interaction/cubicGeneric", GL_FRAGMENT_SHADER);

	rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC] = R_FindProgram("interaction/cubicGeneric", vertexShader, fragmentShader);
	if (!rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC])
		Com_Error(ERR_FATAL, "RB_SetupInteractionShaders: invalid program '%s'", "interaction/cubicGeneric");

	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].viewOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_ViewOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].lightOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].lightAxis = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightAxis", 1, GL_FLOAT_MAT3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].bumpMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_BumpMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].diffuseMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_DiffuseMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].specularMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_SpecularMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].lightMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].colorScaleAndBias = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_ColorScaleAndBias", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].diffuseColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_DiffuseColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].specularColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_SpecularColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].specularParms = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_SpecularParms", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_CUBIC].lightColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightColor", 1, GL_FLOAT_VEC3);

	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_BumpMap", 1, GL_SAMPLER_2D, TMU_BUMP);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_DiffuseMap", 1, GL_SAMPLER_2D, TMU_DIFFUSE);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_SpecularMap", 1, GL_SAMPLER_2D, TMU_SPECULAR);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_CUBIC], "u_LightCubeMap", 1, GL_SAMPLER_CUBE, TMU_LIGHTCUBE);

	// Load projectedGeneric
	vertexShader = R_FindShader("interaction/projectedGeneric", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("interaction/projectedGeneric", GL_FRAGMENT_SHADER);

	rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED] = R_FindProgram("interaction/projectedGeneric", vertexShader, fragmentShader);
	if (!rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED])
		Com_Error(ERR_FATAL, "RB_SetupInteractionShaders: invalid program '%s'", "interaction/projectedGeneric");
#if 0
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].viewOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_ViewOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].lightOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].bumpMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_BumpMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].diffuseMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_DiffuseMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].specularMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_SpecularMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].lightMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].colorScaleAndBias = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_ColorScaleAndBias", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].diffuseColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_DiffuseColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].specularColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_SpecularColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].specularParms = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_SpecularParms", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].lightColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_PROJECTED].lightPlane = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightPlane", 1, GL_FLOAT_VEC4);

	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_BumpMap", 1, GL_SAMPLER_2D, TMU_BUMP);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_DiffuseMap", 1, GL_SAMPLER_2D, TMU_DIFFUSE);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_SpecularMap", 1, GL_SAMPLER_2D, TMU_SPECULAR);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_PROJECTED], "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
#endif
	// Load directionalGeneric
	vertexShader = R_FindShader("interaction/directionalGeneric", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("interaction/directionalGeneric", GL_FRAGMENT_SHADER);

	rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL] = R_FindProgram("interaction/directionalGeneric", vertexShader, fragmentShader);
	if (!rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL])
		Com_Error(ERR_FATAL, "RB_SetupInteractionShaders: invalid program '%s'", "interaction/directionalGeneric");
#if 0
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].viewOrigin = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_ViewOrigin", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].lightDirection = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightDirection", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].bumpMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_BumpMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].diffuseMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_DiffuseMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].specularMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_SpecularMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].lightMatrix = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].colorScaleAndBias = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_ColorScaleAndBias", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].diffuseColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_DiffuseColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].specularColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_SpecularColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].specularParms = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_SpecularParms", 1, GL_FLOAT_VEC2);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].lightColor = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightColor", 1, GL_FLOAT_VEC3);
	backEnd.interactionParms[INTERACTION_GENERIC][RL_DIRECTIONAL].lightPlane = R_GetProgramUniformExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightPlane", 1, GL_FLOAT_VEC4);

	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_BumpMap", 1, GL_SAMPLER_2D, TMU_BUMP);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_DiffuseMap", 1, GL_SAMPLER_2D, TMU_DIFFUSE);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_SpecularMap", 1, GL_SAMPLER_2D, TMU_SPECULAR);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.interactionPrograms[INTERACTION_GENERIC][RL_DIRECTIONAL], "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
#endif
}

/*
 ==================
 RB_SetupAmbientLightShaders
 ==================
*/
static void RB_SetupAmbientLightShaders (){

	shader_t	*vertexShader, *fragmentShader;

	// Load generic
	vertexShader = R_FindShader("ambientLight/generic", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("ambientLight/generic", GL_FRAGMENT_SHADER);

	rg.ambientLightPrograms[AMBIENT_GENERIC] = R_FindProgram("ambientLight/generic", vertexShader, fragmentShader);
	if (!rg.ambientLightPrograms[AMBIENT_GENERIC])
		Com_Error(ERR_FATAL, "RB_SetupAmbientLightShaders: invalid program '%s'", "ambientLight/generic");

	backEnd.ambientLightParms[AMBIENT_GENERIC].bumpMatrix = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_BumpMatrix", 1, GL_FLOAT_MAT4);
	backEnd.ambientLightParms[AMBIENT_GENERIC].diffuseMatrix = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_DiffuseMatrix", 1, GL_FLOAT_MAT4);
	backEnd.ambientLightParms[AMBIENT_GENERIC].lightMatrix = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.ambientLightParms[AMBIENT_GENERIC].colorScaleAndBias = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_ColorScaleAndBias", 1, GL_FLOAT_VEC2);
	backEnd.ambientLightParms[AMBIENT_GENERIC].diffuseColor = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_DiffuseColor", 1, GL_FLOAT_VEC3);
	backEnd.ambientLightParms[AMBIENT_GENERIC].lightColor = R_GetProgramUniformExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_LightColor", 1, GL_FLOAT_VEC3);

	R_SetProgramSamplerExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_BumpMap", 1, GL_SAMPLER_2D, TMU_BUMP);
	R_SetProgramSamplerExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_DiffuseMap", 1, GL_SAMPLER_2D, TMU_DIFFUSE);
	R_SetProgramSamplerExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.ambientLightPrograms[AMBIENT_GENERIC], "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
}

/*
 ==================
 RB_SetupBlendLightShaders
 ==================
*/
static void RB_SetupBlendLightShaders (){

	shader_t	*vertexShader, *fragmentShader;

	// Load generic
	vertexShader = R_FindShader("blendLight/generic", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("blendLight/generic", GL_FRAGMENT_SHADER);

	rg.blendLightProgram = R_FindProgram("blendLight/generic", vertexShader, fragmentShader);
	if (!rg.blendLightProgram)
		Com_Error(ERR_FATAL, "RB_SetupBlendLightShaders: invalid program '%s'", "blendLight/generic");

	backEnd.blendLightParms.lightMatrix = R_GetProgramUniformExplicit(rg.blendLightProgram, "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.blendLightParms.lightColor = R_GetProgramUniformExplicit(rg.blendLightProgram, "u_LightColor", 1, GL_FLOAT_VEC4);

	R_SetProgramSamplerExplicit(rg.blendLightProgram, "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.blendLightProgram, "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
}

/*
 ==================
 RB_SetupFogLightShaders
 ==================
*/
static void RB_SetupFogLightShaders (){

	shader_t	*vertexShader, *fragmentShader;

	// Load generic
	vertexShader = R_FindShader("fogLight/generic", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("fogLight/generic", GL_FRAGMENT_SHADER);

	rg.fogLightProgram = R_FindProgram("fogLight/generic", vertexShader, fragmentShader);
	if (!rg.fogLightProgram)
		Com_Error(ERR_FATAL, "RB_SetupFogLightShaders: invalid program '%s'", "fogLight/generic");

	backEnd.fogLightParms.lightMatrix = R_GetProgramUniformExplicit(rg.fogLightProgram, "u_LightMatrix", 1, GL_FLOAT_MAT4);
	backEnd.fogLightParms.lightColor = R_GetProgramUniformExplicit(rg.fogLightProgram, "u_LightColor", 1, GL_FLOAT_VEC4);

	R_SetProgramSamplerExplicit(rg.fogLightProgram, "u_LightProjectionMap", 1, GL_SAMPLER_2D, TMU_LIGHTPROJECTION);
	R_SetProgramSamplerExplicit(rg.fogLightProgram, "u_LightFalloffMap", 1, GL_SAMPLER_2D, TMU_LIGHTFALLOFF);
}

/*
 ==================
 RB_SetupBlurShaders
 ==================
*/
static void RB_SetupBlurShaders (){

	shader_t	*vertexShader, *fragmentShader;

	if (!r_bloom->integerValue)
		return;

	// Load blur5x5
	vertexShader = R_FindShader("blurFilters/blur5x5", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("blurFilters/blur5x5", GL_FRAGMENT_SHADER);

	rg.blurPrograms[BLUR_5X5] = R_FindProgram("blurFilters/blur5x5", vertexShader, fragmentShader);
	if (!rg.blurPrograms[BLUR_5X5])
		Com_Error(ERR_FATAL, "RB_SetupBlurShaders: invalid program '%s'", "blurFilters/blur5x5");

	backEnd.blurParms[BLUR_5X5].coordScale = R_GetProgramUniformExplicit(rg.blurPrograms[BLUR_5X5], "u_CoordScale", 1, GL_FLOAT_VEC2);

	R_SetProgramSamplerExplicit(rg.blurPrograms[BLUR_5X5], "u_ColorMap", 1, GL_SAMPLER_2D, 0);

	// Load blur9x9
	vertexShader = R_FindShader("blurFilters/blur9x9", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("blurFilters/blur9x9", GL_FRAGMENT_SHADER);

	rg.blurPrograms[BLUR_9X9] = R_FindProgram("blurFilters/blur9x9", vertexShader, fragmentShader);
	if (!rg.blurPrograms[BLUR_9X9])
		Com_Error(ERR_FATAL, "RB_SetupBlurShaders: invalid program '%s'", "blurFilters/blur9x9");

	backEnd.blurParms[BLUR_9X9].coordScale = R_GetProgramUniformExplicit(rg.blurPrograms[BLUR_9X9], "u_CoordScale", 1, GL_FLOAT_VEC2);

	R_SetProgramSamplerExplicit(rg.blurPrograms[BLUR_5X5], "u_ColorMap", 1, GL_SAMPLER_2D, 0);

	// Load blur13x13
	vertexShader = R_FindShader("blurFilters/blur13x13", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("blurFilters/blur13x13", GL_FRAGMENT_SHADER);

	rg.blurPrograms[BLUR_13X13] = R_FindProgram("blurFilters/blur13x13", vertexShader, fragmentShader);
	if (!rg.blurPrograms[BLUR_13X13])
		Com_Error(ERR_FATAL, "RB_SetupBlurShaders: invalid program '%s'", "blurFilters/blur13x13");

	backEnd.blurParms[BLUR_13X13].coordScale = R_GetProgramUniformExplicit(rg.blurPrograms[BLUR_13X13], "u_CoordScale", 1, GL_FLOAT_VEC2);

	R_SetProgramSamplerExplicit(rg.blurPrograms[BLUR_13X13], "u_ColorMap", 1, GL_SAMPLER_2D, 0);

	// Load blur17x17
	vertexShader = R_FindShader("blurFilters/blur17x17", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("blurFilters/blur17x17", GL_FRAGMENT_SHADER);

	rg.blurPrograms[BLUR_17X17] = R_FindProgram("blurFilters/blur17x17", vertexShader, fragmentShader);
	if (!rg.blurPrograms[BLUR_17X17])
		Com_Error(ERR_FATAL, "RB_SetupBlurShaders: invalid program '%s'", "blurFilters/blur17x17");

	backEnd.blurParms[BLUR_17X17].coordScale = R_GetProgramUniformExplicit(rg.blurPrograms[BLUR_17X17], "u_CoordScale", 1, GL_FLOAT_VEC2);

	R_SetProgramSamplerExplicit(rg.blurPrograms[BLUR_17X17], "u_ColorMap", 1, GL_SAMPLER_2D, 0);
}

/*
 ==================
 RB_SetupPostProcessShaders
 ==================
*/
static void RB_SetupPostProcessShaders (){

	shader_t	*vertexShader, *fragmentShader;

	if (!r_postProcess->integerValue)
		return;

	// Load bloom
	if (r_bloom->integerValue){
		vertexShader = R_FindShader("postProcess", GL_VERTEX_SHADER);
		fragmentShader = R_FindShader("bloom", GL_FRAGMENT_SHADER);

		rg.bloomProgram = R_FindProgram("bloom", vertexShader, fragmentShader);
		if (!rg.bloomProgram)
			Com_Error(ERR_FATAL, "RB_SetupPostProcessShaders: invalid program '%s'", "bloom");

		backEnd.bloomParms.stOffset1 = R_GetProgramUniformExplicit(rg.bloomProgram, "u_STOffset1", 1, GL_FLOAT_VEC2);
		backEnd.bloomParms.stOffset2 = R_GetProgramUniformExplicit(rg.bloomProgram, "u_STOffset2", 1, GL_FLOAT_VEC2);
		backEnd.bloomParms.stOffset3 = R_GetProgramUniformExplicit(rg.bloomProgram, "u_STOffset3", 1, GL_FLOAT_VEC2);
		backEnd.bloomParms.bloomContrast = R_GetProgramUniformExplicit(rg.bloomProgram, "u_BloomContrast", 1, GL_FLOAT);
		backEnd.bloomParms.bloomThreshold = R_GetProgramUniformExplicit(rg.bloomProgram, "u_BloomThreshold", 1, GL_FLOAT);

		R_SetProgramSamplerExplicit(rg.bloomProgram, "u_ColorMap", 1, GL_SAMPLER_2D, 0);
	}

	// Load colorCorrection
	vertexShader = R_FindShader("postProcess", GL_VERTEX_SHADER);
	fragmentShader = R_FindShader("colorCorrection", GL_FRAGMENT_SHADER);

	rg.colorCorrectionProgram = R_FindProgram("colorCorrection", vertexShader, fragmentShader);
	if (!rg.colorCorrectionProgram)
		Com_Error(ERR_FATAL, "RB_SetupPostProcessShaders: invalid program '%s'", "colorCorrection");

	backEnd.colorCorrectionParms.baseIntensity = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_BaseIntensity", 1, GL_FLOAT);
	backEnd.colorCorrectionParms.glowIntensity = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_GlowIntensity", 1, GL_FLOAT);
	backEnd.colorCorrectionParms.colorShadows = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorShadows", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorHighlights = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorHighlights", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorMidtones = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorMidtones", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorMinOutput = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorMinOutput", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorMaxOutput = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorMaxOutput", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorSaturation = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorSaturation", 1, GL_FLOAT_VEC3);
	backEnd.colorCorrectionParms.colorTint = R_GetProgramUniformExplicit(rg.colorCorrectionProgram, "u_ColorTint", 1, GL_FLOAT_VEC3);

	R_SetProgramSamplerExplicit(rg.colorCorrectionProgram, "u_BaseMap", 1, GL_SAMPLER_2D, 0);
	R_SetProgramSamplerExplicit(rg.colorCorrectionProgram, "u_GlowMap", 1, GL_SAMPLER_2D, 1);
	R_SetProgramSamplerExplicit(rg.colorCorrectionProgram, "u_ColorTable", 1, GL_SAMPLER_2D, 2);
}


/*
 ==============================================================================

 INITIALIZATION AND SHUTDOWN

 ==============================================================================
*/


/*
 ==================
 RB_InitBackEnd
 ==================
*/
void RB_InitBackEnd (){

	// Allocate the render command buffer
	backEnd.commandBuffer.data = (byte *)Mem_Alloc(MAX_COMMAND_BUFFER_SIZE, TAG_RENDERER);

	// Allocate index and vertex arrays
	backEnd.indices = (glIndex_t *)Mem_Alloc(MAX_INDICES * sizeof(glIndex_t), TAG_RENDERER);
	backEnd.vertices = (glVertex_t *)Mem_Alloc16(MAX_VERTICES * sizeof(glVertex_t), TAG_RENDERER);

	backEnd.shadowIndices = (glIndex_t *)Mem_Alloc(MAX_SHADOW_INDICES * sizeof(glIndex_t), TAG_RENDERER);
	backEnd.shadowVertices = (glShadowVertex_t *)Mem_Alloc16(MAX_SHADOW_VERTICES * sizeof(glVertex_t), TAG_RENDERER);

	// Allocate dynamic vertex buffer
	backEnd.dynamicIndexBuffers[0] = R_AllocIndexBuffer("streamBuffer1", true, MAX_DYNAMIC_INDICES, NULL);
	backEnd.dynamicIndexBuffers[1] = R_AllocIndexBuffer("streamBuffer2", true, MAX_DYNAMIC_INDICES, NULL);

	backEnd.dynamicVertexBuffers[0] = R_AllocVertexBuffer("streamBuffer1", true, MAX_DYNAMIC_VERTICES, NULL);
	backEnd.dynamicVertexBuffers[1] = R_AllocVertexBuffer("streamBuffer2", true, MAX_DYNAMIC_VERTICES, NULL);

	// Set up shaders
	RB_SetupInteractionShaders();
	RB_SetupAmbientLightShaders();
	RB_SetupBlendLightShaders();
	RB_SetupFogLightShaders();
	RB_SetupBlurShaders();
	RB_SetupPostProcessShaders();
}

/*
 ==================
 RB_ShutdownBackEnd
 ==================
*/
void RB_ShutdownBackEnd (){

	// Clear the back-end structure
	Mem_Fill(&backEnd, 0, sizeof(backEnd_t));
}