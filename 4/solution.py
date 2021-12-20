import numpy as np


def check_win(board):
    horizontal = np.sum(board, axis=1)
    vertical = np.sum(board, axis=0)

    return (-5 in horizontal) or (-5 in vertical)


def part_1(bingo_numbers, bingo_boards):
    for num in bingo_numbers:
        for board in bingo_boards:
            board[board == num] = -1
            if check_win(board):
                board[board == -1] = 0
                sum = np.sum(board)
                return (sum, num)


def part_2(bingo_numbers, bingo_boards):
    boards_left = len(bingo_boards)
    winners = {}
    for num in bingo_numbers:
        board_id = 0
        for board in bingo_boards:
            board[board == num] = -1
            if board_id not in winners and check_win(board):
                winners[board_id] = True
                boards_left -= 1
                if boards_left == 0:
                    board[board == -1] = 0
                    sum = np.sum(board)
                    return (sum, num)
            board_id += 1


with open("data.in", "r") as f:
    numbers = [int(v) for v in f.readline().split(',')]
    f.readline()

    boards = []
    current_board = []
    for line in f:
        line = line.strip()
        if (line == ''):
            boards.append(np.array([np.array(v) for v in current_board]))
            current_board = []
        else:
            current_board.append([int(v) for v in line.split(' ') if v != ''])

    [s, n] = part_1(numbers, np.copy(boards))
    print(s * n)
    [s, n] = part_2(numbers, boards)
    print(s * n)
