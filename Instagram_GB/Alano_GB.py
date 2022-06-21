import numpy as np
import sys
import cv2

# Criando as funçoes para cada filtro
def app_blur(frame):
    return cv2.GaussianBlur(frame, (9,9),0)

def app_negativo(frame):
    return cv2.bitwise_not(frame)

def verify_alpha(frame):
    try:
        frame.shape[3]
    except IndexError:
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)
        return frame

def app_sepia(frame, intensidade=0.9):
    frame = verify_alpha(frame)
    frame_h, frame_w, frame_c = frame.shape
    blue = 20
    green = 66
    red = 112
    sepia_bgra = (blue, green, red, 1)
    overlay = np.full((frame_h, frame_w, 4), sepia_bgra, dtype='uint8')
    cv2.addWeighted(overlay, intensidade, frame, 1.0, 0, frame)
    
    frame = cv2.cvtColor(frame, cv2.COLOR_BGRA2BGR)
    return frame

def app_contraste(frame):
    frame_LAB = cv2.cvtColor(frame, cv2.COLOR_BGR2LAB)
    L, A, B = cv2.split(frame_LAB)
    clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8,8))
    cl1 = clahe.apply(L)
    frame_clahe = cv2.merge((cl1, A, B))
    CLAHE_frame = cv2.cvtColor(frame_clahe, cv2.COLOR_LAB2BGR)
    return CLAHE_frame

def app_canny(frame):
    return cv2.Canny(frame, 100,200)

def app_binario(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    res, frame_bi = cv2.threshold(gray, 120, 255, cv2.THRESH_BINARY)
    return frame_bi

def app_desenho(frame):
    sk_gray, sk_color = cv2.pencilSketch(frame, sigma_s=60, sigma_r=0.07, shade_factor=0.05) 
    return  sk_color

def app_pintura(frame):
    return cv2.stylization(frame, sigma_s=60, sigma_r=0.07)

def app_afiado(frame):
    kernel = np.array([[-1, -1, -1], [-1, 9.5, -1], [-1, -1, -1]])
    frame_afiado = cv2.filter2D(frame, -1, kernel)
    return frame_afiado

def app_cinza(frame):
    return cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

# Prototipo dos stickers
def draw_sticker(event,x,y,flags,param):
    pig = cv2.imread("pig.png")
    pigsize = cv2.resize(pig, (80, 80))
    s_img = pigsize
    l_img = cv2.imread("messi.png")
    if event == cv2.EVENT_LBUTTONDBLCLK:
        x_offset= x
        y_offset= y
        l_img[y_offset:y_offset+s_img.shape[0], x_offset:x_offset+s_img.shape[1]] = s_img
        while True:
            cv2.imshow('image', l_img)
            if cv2.waitKey(1) == 27:
                break

# Checar se webcam abre corretamente
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    raise IOError("Erro ao abrir webcam")

# Lista pros filtros
filters = []

# Escolhendo o modo Imagem ou Webcam
modo = input("Aperte 1 para usar uma imagem e 2 para usar a webcam: ")
if modo == '1':
    print("Utilizando modo Imagem")
    imagem = input("Digite o nome da imagem desejada: ")

if modo == '2':
    print("Utilizando modo WebCam")


cv2.namedWindow('Tela')
cv2.setMouseCallback('Tela', draw_sticker)

# Executando os filtros
while True:
    if modo == '1':
        img = cv2.imread(imagem+'.png')
    if modo == '2':
        ret, img = cap.read()
        img = cv2.resize(img, None, fx=0.75, fy=0.75, interpolation=cv2.INTER_AREA)
    
    key = cv2.waitKey(1)

    if key == ord('1'):
        if app_blur in filters:
            filters.remove(app_blur)
        else:
            filters.append(app_blur)

    if key == ord('2'):
        if app_negativo in filters:
            filters.remove(app_negativo)
        else:
            filters.append(app_negativo)

    if key == ord('3'):
        if app_sepia in filters:
            filters.remove(app_sepia)
        else:
            filters.append(app_sepia)

    if key == ord('4'):
        if app_contraste in filters:
            filters.remove(app_contraste)
        else:
            filters.append(app_contraste)

    if key == ord('5'):
        if app_canny in filters:
            filters.remove(app_canny)
        else:
            filters.append(app_canny)

    if key == ord('6'):
        if app_binario in filters:
            filters.remove(app_binario)
        else:
            filters.append(app_binario)

    if key == ord('7'):
        if app_desenho in filters:
            filters.remove(app_desenho)
        else:
            filters.append(app_desenho)

    if key == ord('8'):
        if app_pintura in filters:
            filters.remove(app_pintura)
        else:
            filters.append(app_pintura)

    if key == ord('9'):
        if app_afiado in filters:
            filters.remove(app_afiado)
        else:
            filters.append(app_afiado)

    if key == ord('0'):
        if app_cinza in filters:
            filters.remove(app_cinza)
        else:
            filters.append(app_cinza)

    for filter in filters:
        img = filter(img)

    cv2.imshow('Tela', img)

    if key == 27:
        break
            
cap.release()
cv2.destroyAllWindows()