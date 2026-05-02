/*
  ==============================================================================

    Processor.h
    Created: 30 Apr 2026 5:23:50pm
    Author:  Alberto

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

/**
* Interfaz que expone los métodos comunes a todas las clases del modelo que procesan audio
**/
class ProcessorBase {
public:
    virtual ~ProcessorBase() {}

    /**
    * Prepara el estado de los objetos, reserva memoria y establece los valores iniciales para el correcto
    * procesamiento del audio. Llama a este método fuera de processBlock.
    * @param juce::dsp::ProcessSpec spec : especificaciones del procesamiento de audio para este bloque.
    **/
    virtual void prepare(juce::dsp::ProcessSpec& spec) = 0;

    /**
    * Actualiza el estado del objeto según los valores actuales del plugin. En la medida de lo posible llama a este método
    * dentro de processBlock.
    * @param parameters::Parameters parameters : referencia a los parámetros del plugin.
    **/
    virtual void update(parameters::Parameters& parameters) = 0;

    /**
    * Procesa el audio de entrada y salida produciendo un resultado que se guarda dentro del buffer de salida de context.
    * @param juce::dsp::ProcessContextReplacing<float>& context : objeto que encapsula punteros hacia el bloque de audio que se está procesando.
    **/
    virtual void process(juce::dsp::ProcessContextReplacing<float>& context) = 0;

    /**
    * Resetea el estado y limpia los posibles buffers de audio. No llames a este método dentro de processBlock
    **/
    virtual void reset() = 0;
};