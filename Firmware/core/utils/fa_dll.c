/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  Future Automation Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    fa_dll.c

  Summary:
    This file contains the source code for the Future Automation RS485 interface.

  Description:
    This file contains the source code for the Future Automation RS485 interface.  
    This provides all transport layer and command-set functionality 

 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "fa_global.h"
#include "fa_app.h"
#include "fa_sdn_api.h"
#include "fa_dll.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Global Definitions
// *****************************************************************************
// *****************************************************************************





// *****************************************************************************
// *****************************************************************************
// Section: Forward references
// *****************************************************************************
// *****************************************************************************



/*******************************************************************************
  Function:
   

  Remarks:
    
    
 */

void FA_DLL_Insert_at_Head(struct Node **head, uint32_t data) {
  struct Node *current = *head;
  while (current != NULL) {
    if (current->data == data) {
      // data already exists in the linked list, return
      return;
    }
    current = current->next;
  }

  // data does not exist in the linked list, proceed with insertion
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  new_node->data = data;
  new_node->next = *head;
  if (*head != NULL) {
    (*head)->prev = new_node;
  }
  new_node->prev = NULL;
  *head = new_node;
}

struct Node* FA_DLL_Get_Next_Element(struct Node *head, struct Node *current) {
    if (current->next != NULL) {
        return current->next;
    } else {
        return head;
    }
}

void FA_DLL_Delete_Node_By_Value(struct Node **head, uint32_t value) {
  struct Node *current = *head;
  while (current != NULL) {
    if (current->data == value) {
      if (current->prev != NULL) {
        current->prev->next = current->next;
      } else {
        *head = current->next;
        if (current->next != NULL) {
          current->next->prev = NULL;
        }
      }
      if (current->next != NULL) {
        current->next->prev = current->prev;
      }
      struct Node *temp = current->next;
      free(current);
      current = temp;
      continue;
    }
    current = current->next;
  }
}





void FA_DLL_Delete_Node(struct Node **head, struct Node *to_delete) {
    if (*head == NULL || to_delete == NULL) {
        return;
    }

    if (*head == to_delete) {
        *head = (*head)->next;
    }

    if (to_delete->next != NULL) {
        to_delete->next->prev = to_delete->prev;
    }

    if (to_delete->prev != NULL) {
        to_delete->prev->next = to_delete->next;
    }
    free(to_delete);
}