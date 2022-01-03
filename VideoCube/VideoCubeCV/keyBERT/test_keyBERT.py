# -*- coding: UTF-8 -*-
from keybert import KeyBERT

doc22 = """
         Supervised learning is the machine learning task of learning a function that
         maps an input to an output based on example input-output pairs. It infers a
         function from labeled training data consisting of a set of training examples.
         In supervised learning, each example is a pair consisting of an input object
         (typically a vector) and a desired output value (also called the supervisory signal). 
         A supervised learning algorithm analyzes the training data and produces an inferred function, 
         which can be used for mapping new examples. An optimal scenario will allow for the 
         algorithm to correctly determine the class labels for unseen instances. This requires 
         the learning algorithm to generalize from the training data to unseen situations in a 
         'reasonable' way (see inductive bias).
      """
doc = """САНКТ-ПЕТЕРБУРГ, 3 января. /ТАСС/. Власти в Петербурге возбудили 19 административных дел по факту несвоевременного вывоза мусора и уборки контейнерных площадок. Об этом сообщили в пресс-службе государственной жилищной инспекции Петербурга.

"По результатам проверок возбуждено 19 административных дел на общую сумму штрафных санкций свыше 2 млн рублей", - сказано в сообщении.

Всего было проверено более 400 многоквартирных домов. Основными нарушениями стали несвоевременный вывоз мусора, несвоевременная уборка контейнерных площадок и придомовых территорий, а также непринятие мер по устранению наледи с кровель МКД. 37% нарушений приходятся на неубранные контейнерные площадки.

В декабре власти Ленинградской области определили четыре площадки для приема твердых бытовых отходов из Санкт-Петербурга в рамках взаимодействия двух регионов по обращению с отходами. До 2030 года Ленинградская область планирует сократить объем размещения отходов до 50% и до 80% увеличить долю отходов, направляемых на переработку.
"""

kw_model = KeyBERT()
keywords = kw_model.extract_keywords(doc)
print(keywords)
